#include <CQPerfMonitor.h>
#include <CEnv.h>

CQPerfMonitor::
CQPerfMonitor()
{
  CEnvInst.get("CQ_PERF_MONITOR_ENABLED", enabled_);
  CEnvInst.get("CQ_PERF_MONITOR_DEBUG"  , debug_  );
}

void
CQPerfMonitor::
setEnabled(bool b)
{
  enabled_ = b;

  emit stateChanged();
}

void
CQPerfMonitor::
setDebug(bool b)
{
  debug_ = b;

  emit stateChanged();
}

void
CQPerfMonitor::
startTrace(const QString &name)
{
  CQPerfTraceData *data = getTrace(name);

  if (data->isEnabled()) {
    ++numTrace_;

    data->startTrace(numTrace_);
  }
}

void
CQPerfMonitor::
endTrace(const QString &name)
{
  CQPerfTraceData *data = getTrace(name);

  if (data->isEnabled()) {
    data->endTrace();

    --numTrace_;
  }
}

void
CQPerfMonitor::
startDebug(const QString &name)
{
  CQPerfTraceData *data = getTrace(name);

  if (data->isDebug()) {
    ++numDebug_;

    QString msg = QString(">%1%2").arg(" ", numDebug_).arg(name);

    std::cerr << msg.toStdString() << "\n";

    data->startDebug();
  }
}

void
CQPerfMonitor::
endDebug(const QString &name)
{
  CQPerfTraceData *data = getTrace(name);

  if (data->isDebug()) {
    data->endDebug();

    const CHRTime &e = data->elapsedTime();

    QString msg = QString("<%1%2 %3").arg(" ", numDebug_).arg(name).arg(e.getSecs(), 0, 'f', 6);

    std::cerr << msg.toStdString() << "\n";

    --numDebug_;
  }
}

void
CQPerfMonitor::
resetTrace(const QString &name)
{
  CQPerfTraceData *data = getTrace(name);

  data->reset();
}

void
CQPerfMonitor::
resetAll()
{
  for (auto &nt : traces_)
    nt.second->reset();
}

void
CQPerfMonitor::
startRecording()
{
  recording_ = true;

  for (auto &nt : traces_)
    nt.second->startRecording();
}

void
CQPerfMonitor::
stopRecording()
{
  for (auto &nt : traces_)
    nt.second->stopRecording();

  recording_ = false;
}

bool
CQPerfMonitor::
isTraceEnabled(const QString &name)
{
  CQPerfTraceData *data = getTrace(name);

  return data->isEnabled();
}

void
CQPerfMonitor::
setTraceEnabled(const QString &name, bool enabled)
{
  CQPerfTraceData *data = getTrace(name);

  data->setEnabled(enabled);
}

bool
CQPerfMonitor::
isTraceDebug(const QString &name)
{
  CQPerfTraceData *data = getTrace(name);

  return data->isDebug();
}

void
CQPerfMonitor::
setTraceDebug(const QString &name, bool debug)
{
  CQPerfTraceData *data = getTrace(name);

  data->setDebug(debug);
}

void
CQPerfMonitor::
setTraceMaxTime(const QString &name, const CHRTime &t)
{
  CQPerfTraceData *data = getTrace(name);

  data->setMaxTime(t);
}

void
CQPerfMonitor::
setTraceMaxCalls(const QString &name, int n)
{
  CQPerfTraceData *data = getTrace(name);

  data->setMaxCalls(n);
}

void
CQPerfMonitor::
reportStats(const QString &name)
{
  CQPerfTraceData *data = getTrace(name);

  data->reportStats();
}

void
CQPerfMonitor::
getTraceNames(QStringList &names)
{
  for (auto &nt : traces_)
    names.push_back(nt.second->name());
}

CQPerfTraceData *
CQPerfMonitor::
getTrace(const QString &name)
{
  Traces::iterator p = traces_.find(name);

  if (p == traces_.end()) {
    CQPerfTraceData *traceData = new CQPerfTraceData(name);

    p = traces_.insert(p, Traces::value_type(name, traceData));

    if (recording_)
      traceData->startRecording();

    emit traceAdded(name);
  }

  return (*p).second;
}

//---

CQPerfTraceData::
CQPerfTraceData(const QString &name) :
 name_(name)
{
  std::string pattern;

  CEnvInst.get("CQ_PERF_MONITOR_DEBUG_PATTERN", pattern);

  QStringList patterns = QString(pattern.c_str()).split("|", QString::SkipEmptyParts);

  for (int i = 0; i < patterns.length(); ++i) {
    QRegExp regexp(patterns[i], Qt::CaseSensitive, QRegExp::WildcardUnix);

    if (regexp.exactMatch(name)) {
      debug_ = true;
      break;
    }
  }
}

void
CQPerfTraceData::
startTrace(int depth)
{
  timeData_.depth = depth;
  timeData_.start = CHRTime::getTime();
}

void
CQPerfTraceData::
endTrace()
{
  // calc elapsed time
  CHRTime endTime = CHRTime::getTime();

  timeData_.elapsed = CHRTime::diffTime(timeData_.start, endTime);

  //---

  // update number of calls, total time, max and min time
  ++calls_;

  elapsed_ += timeData_.elapsed;

  if (elapsedMin_.isSet()) {
    elapsedMin_ = std::min(elapsedMin_, timeData_.elapsed);
    elapsedMax_ = std::max(elapsedMax_, timeData_.elapsed);
  }
  else {
    elapsedMin_ = timeData_.elapsed;
    elapsedMax_ = timeData_.elapsed;
  }

  //---

  // get limits of window count and time
  int     windowCount = CQPerfMonitorInst->windowCount();
  CHRTime windowTime  = CQPerfMonitorInst->windowTime ();

  //---

  // add new time
  int nt = times_.size();

  posStart_ = posStartNext_;

  if (windowCount > 0) {
    if (nt >= windowCount) {
      ++posStartNext_;

      if (posStartNext_ >= nt)
        posStartNext_ -= nt;
    }
  }

  //---

  if (windowCount > 0) {
    if (nt < windowCount)
      times_.push_back(timeData_);
    else {
      setTimeData(posEnd(), timeData_);
    }
  }
  else
    times_.push_back(timeData_);

  if (recording_)
    recordTimes_.push_back(timeData_);

  //---

  if (windowTime.isSet()) {
    int i = 0;
    int n = windowCount;

    for ( ; i < n; ++i) {
      int pos = fixPos(posStart_ + i);

      if (timeData(pos).start >= windowTime)
        break;
    }
  }

  //---

  if (maxCalls_ > 0 && calls_ > maxCalls_)
    assert(false);

  if (maxTime_.isSet() && elapsedMax_ > maxTime_)
    assert(false);
}

void
CQPerfTraceData::
startDebug(int depth)
{
  timeData_.depth = depth;
  timeData_.start = CHRTime::getTime();
}

void
CQPerfTraceData::
endDebug()
{
  // calc elapsed time
  CHRTime endTime = CHRTime::getTime();

  timeData_.elapsed = CHRTime::diffTime(timeData_.start, endTime);

  if (! isEnabled() || ! CQPerfMonitorInst->isEnabled()) {
    ++calls_;

    elapsed_ += timeData_.elapsed;

    if (elapsedMin_.isSet()) {
      elapsedMin_ = std::min(elapsedMin_, timeData_.elapsed);
      elapsedMax_ = std::max(elapsedMax_, timeData_.elapsed);
    }
    else {
      elapsedMin_ = timeData_.elapsed;
      elapsedMax_ = timeData_.elapsed;
    }
  }
}

void
CQPerfTraceData::
reset()
{
  times_.clear();

  posStart_     = 0;
  posStartNext_ = 0;

  calls_      = 0;
  elapsed_    = CHRTime();
  elapsedMin_ = CHRTime();
  elapsedMax_ = CHRTime();

}

void
CQPerfTraceData::
startRecording()
{
  recording_ = true;

  recordTimes_.clear();
}

void
CQPerfTraceData::
stopRecording()
{
  recording_ = false;
}

const CHRTime &
CQPerfTraceData::
windowStartTime() const
{
  int pos = fixPos(posStart_);

  return timeData(pos).start;
}

const CHRTime &
CQPerfTraceData::
windowEndTime() const
{
  int pos = posEnd();

  return timeData(pos).start;
}

void
CQPerfTraceData::
windowDetails(WindowData &windowData) const
{
  int n = windowSize();

  for (int i = 0; i < n; ++i) {
    int pos = fixPos(posStart_ + i);

    const TimeData &data = timeData(pos);

    if (windowData.minT.isSet())
      windowData.minT = std::min(windowData.minT, data.start);
    else
      windowData.minT = data.start;

    if (windowData.maxT.isSet())
      windowData.maxT = std::max(windowData.maxT, data.start + data.elapsed);
    else
      windowData.maxT = data.start + data.elapsed;

    ++windowData.numCalls;

    windowData.elapsed += data.elapsed;
  }
}

void
CQPerfTraceData::
windowDetails(const CHRTime &t1, const CHRTime &t2, WindowData &windowData) const
{
  int n = windowSize();

  for (int i = 0; i < n; ++i) {
    int pos = fixPos(posStart_ + i);

    const TimeData &data = timeData(pos);

    if (data.start >= t1 && data.start <= t2) {
      if (windowData.minT.isSet())
        windowData.minT = std::min(windowData.minT, data.start);
      else
        windowData.minT = data.start;

      if (windowData.maxT.isSet())
        windowData.maxT = std::max(windowData.maxT, data.start + data.elapsed);
      else
        windowData.maxT = data.start + data.elapsed;

      ++windowData.numCalls;

      windowData.elapsed += data.elapsed;
    }
  }
}

void
CQPerfTraceData::
windowDetails(const CHRTime &t1, const CHRTime &t2, TimeDatas &timeDatas) const
{
  int n = windowSize();

  for (int i = 0; i < n; ++i) {
    int pos = fixPos(posStart_ + i);

    const TimeData &data = timeData(pos);

    if (data.start >= t1 && data.start <= t2)
      timeDatas.push_back(data);
  }
}

void
CQPerfTraceData::
reportStats()
{
  std::cerr << "Calls:    " << calls_      << "\n";
  std::cerr << "Min Time: " << elapsedMin_ << "\n";
  std::cerr << "Max Time: " << elapsedMax_ << "\n";
}

int
CQPerfTraceData::
posEnd() const
{
  int nt = times_.size();

  int posEnd = posStart_ + nt;

  if (posEnd >= nt)
    posEnd -= nt;

  return posEnd;
}

int
CQPerfTraceData::
fixPos(int i) const
{
  int nt = times_.size();

  while (i <  0 ) i += nt;
  while (i >= nt) i -= nt;

  return i;
}

int
CQPerfTraceData::
windowSize() const
{
  return times_.size();
}
