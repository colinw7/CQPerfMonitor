#include <CQPerfMonitor.h>

#ifdef CQPERF_MESSAGE
#include <CMessage.h>
#endif

#include <CEnv.h>

#include <QTimer>

CQPerfMonitor::
CQPerfMonitor()
{
  CEnvInst.get("CQ_PERF_MONITOR_ENABLED" , enabled_);
  CEnvInst.get("CQ_PERF_MONITOR_DEBUG"   , debug_  );
  CEnvInst.get("CQ_PERF_MONITOR_MIN_TIME", minTime_);
}

CQPerfMonitor::
~CQPerfMonitor()
{
#ifdef CQPERF_MESSAGE
  delete message_;
#endif
}

//---

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

//---

void
CQPerfMonitor::
createServer(const QString &name)
{
#ifdef CQPERF_MESSAGE
  std::string msgName = "CQ_PERF_MONITOR";

  if (name != "")
    msgName = name.toStdString();

  assert(! message_);

  message_ = new CMessage(msgName);
  server_  = true;

  serverTimer_ = new QTimer(this);

  connect(serverTimer_, SIGNAL(timeout()), this, SLOT(serverSlot()));

  serverTimer_->start(5);
#else
  std::cerr << "No server support for " << name.toStdString() << "\n";
#endif
}

void
CQPerfMonitor::
createClient(const QString &name)
{
#ifdef CQPERF_MESSAGE
  std::string msgName = "CQ_PERF_MONITOR";

  if (name != "")
    msgName = name.toStdString();

  assert(! message_);

  message_ = new CMessage(msgName);
  server_  = false;

  clientTimer_ = new QTimer(this);

  connect(clientTimer_, SIGNAL(timeout()), this, SLOT(clientSlot()));

  clientTimer_->start(5);
#else
  std::cerr << "No client support for " << name.toStdString() << "\n";
#endif
}

void
CQPerfMonitor::
serverSlot()
{
#ifdef CQPERF_MESSAGE
  std::string msg;

  if (message_->recvClientMessage(msg)) {
    if      (msg[0] == '>')
      startTrace(QString(msg.substr(1).c_str()));
    else if (msg[0] == '<')
      endTrace(QString(msg.substr(1).c_str()));
  }
#endif
}

void
CQPerfMonitor::
clientSlot()
{
#ifdef CQPERF_MESSAGE
  message_->sendClientPending();
#endif
}

//---

void
CQPerfMonitor::
startTrace(const QString &name, TraceType)
{
#ifdef CQPERF_MESSAGE
  if (message_ && ! server_)
    message_->sendClientMessage(">" + name.toStdString());
#endif

  auto *data = getTrace(name);

  if (data->isEnabled()) {
    std::unique_lock<std::mutex> lock(mutex_);

    ++numTrace_;

    data->startTrace(numTrace_);
  }
}

void
CQPerfMonitor::
endTrace(const QString &name, TraceType traceType)
{
#ifdef CQPERF_MESSAGE
  if (message_ && ! server_)
    message_->sendClientMessage("<" + name.toStdString());
#endif

  auto *data = getTrace(name);

  if (data->isEnabled()) {
    std::unique_lock<std::mutex> lock(mutex_);

    data->endTrace(traceType);

    --numTrace_;
  }
}

void
CQPerfMonitor::
addTrace(const QString &name, const TimeData &timeData, TraceType traceType)
{
  auto *data = getTrace(name);

  if (data->isEnabled()) {
    std::unique_lock<std::mutex> lock(mutex_);

    data->addTrace(timeData, traceType);

    --numTrace_;
  }
}

void
CQPerfMonitor::
startDebug(const QString &name)
{
  auto *data = getTrace(name);

  if (data->isDebug()) {
    std::unique_lock<std::mutex> lock(mutex_);

    ++numDebug_;

    if (minTime() > 0) {
      names_.push_back(name);
    }
    else {
      auto msg = QString(">%1%2").arg(" ", numDebug_).arg(name);

      std::cerr << msg.toStdString() << "\n";
    }

    data->startDebug();
  }
}

void
CQPerfMonitor::
endDebug(const QString &name)
{
  auto *data = getTrace(name);

  if (data->isDebug()) {
    std::unique_lock<std::mutex> lock(mutex_);

    data->endDebug();

    const CHRTime &e = data->elapsedTime();

    if (minTime() > 0) {
      if (e.getMSecs() >= minTime()) {
        int n = names_.size();

        int nd = numDebug_ - n + 1;

        for (int i = 0; i < n; ++i) {
          const auto &name = names_[i];

          auto smsg = QString(">%1%2").arg(" ", nd).arg(name);

          std::cerr << smsg.toStdString() << "\n";

          ++nd;
        }

        --nd;

        for (int i = n - 1; i >= 0; --i) {
          const auto &name = names_[i];

          auto emsg = QString("<%1%2 %3").arg(" ", nd).arg(name).arg(e.getSecs(), 0, 'f', 6);

          std::cerr << emsg.toStdString() << "\n";

          --nd;
        }

        names_.clear();
      }
      else {
        names_.pop_back();
      }
    }
    else {
      auto msg = QString("<%1%2 %3").arg(" ", numDebug_).arg(name).arg(e.getSecs(), 0, 'f', 6);

      std::cerr << msg.toStdString() << "\n";
    }

    --numDebug_;
  }
}

void
CQPerfMonitor::
addDebug(const QString &name, const TimeData &timeData)
{
  auto *data = getTrace(name);

  if (data->isDebug()) {
    std::unique_lock<std::mutex> lock(mutex_);

    data->addDebug(timeData);

    const CHRTime &e = timeData.elapsed;

    auto msg = QString("%1%2 %3").arg(" ", numDebug_).arg(name).arg(e.getSecs(), 0, 'f', 6);

    std::cerr << msg.toStdString() << "\n";
  }
}

void
CQPerfMonitor::
resetTrace(const QString &name)
{
  auto *data = getTrace(name);

  std::unique_lock<std::mutex> lock(mutex_);

  data->reset();
}

void
CQPerfMonitor::
resetStartsWith(const QString &name)
{
  std::unique_lock<std::mutex> lock(mutex_);

  for (auto &nt : traces_) {
    if (nt.second->name().startsWith(name))
      nt.second->reset();
  }
}

void
CQPerfMonitor::
resetAll()
{
  std::unique_lock<std::mutex> lock(mutex_);

  for (auto &nt : traces_)
    nt.second->reset();
}

void
CQPerfMonitor::
startRecording()
{
  std::unique_lock<std::mutex> lock(mutex_);

  recording_ = true;

  for (auto &nt : traces_)
    nt.second->startRecording();
}

void
CQPerfMonitor::
stopRecording()
{
  std::unique_lock<std::mutex> lock(mutex_);

  for (auto &nt : traces_)
    nt.second->stopRecording();

  recording_ = false;
}

bool
CQPerfMonitor::
isTraceEnabled(const QString &name) const
{
  auto *data = const_cast<CQPerfMonitor *>(this)->getTrace(name);

  return data->isEnabled();
}

void
CQPerfMonitor::
setTraceEnabled(const QString &name, bool enabled)
{
  auto *data = getTrace(name);

  std::unique_lock<std::mutex> lock(mutex_);

  data->setEnabled(enabled);
}

bool
CQPerfMonitor::
isTraceDebug(const QString &name) const
{
  auto *data = const_cast<CQPerfMonitor *>(this)->getTrace(name);

  return data->isDebug();
}

void
CQPerfMonitor::
setTraceDebug(const QString &name, bool debug)
{
  auto *data = getTrace(name);

  std::unique_lock<std::mutex> lock(mutex_);

  data->setDebug(debug);
}

void
CQPerfMonitor::
setTraceMaxTime(const QString &name, const CHRTime &t)
{
  auto *data = getTrace(name);

  std::unique_lock<std::mutex> lock(mutex_);

  data->setMaxTime(t);
}

void
CQPerfMonitor::
setTraceMaxCalls(const QString &name, int n)
{
  auto *data = getTrace(name);

  std::unique_lock<std::mutex> lock(mutex_);

  data->setMaxCalls(n);
}

void
CQPerfMonitor::
reportStats(const QString &name) const
{
  auto *data = const_cast<CQPerfMonitor *>(this)->getTrace(name);

  data->reportStats();
}

void
CQPerfMonitor::
getTraceNames(QStringList &names) const
{
  std::unique_lock<std::mutex> lock(mutex_);

  for (auto &nt : traces_)
    names.push_back(nt.second->name());
}

void
CQPerfMonitor::
getTracesStartingWith(const QString &name, TraceList &traces)
{
  std::unique_lock<std::mutex> lock(mutex_);

  for (auto &nt : traces_) {
    if (nt.second->name().startsWith(name))
      traces.push_back(nt.second);
  }
}

CQPerfTraceData *
CQPerfMonitor::
getTrace(const QString &name)
{
  bool added = false;

  Traces::iterator p = traces_.find(name);

  if (p == traces_.end()) {
    std::unique_lock<std::mutex> lock(mutex_);

    auto *traceData = new CQPerfTraceData(name);

    p = traces_.insert(p, Traces::value_type(name, traceData));

    if (recording_)
      traceData->startRecording();

    added = true;
  }

  if (added)
    emit traceAdded(name);

  return (*p).second;
}

void
CQPerfMonitor::
alert(const CQPerfTraceData *trace, CQPerfMonitor::AlertType type)
{
  // TODO: customize behavior
  std::cerr << "Alert: " << trace->name().toStdString();
  if      (type == AlertType::CALLS) std::cerr << " (Calls)";
  else if (type == AlertType::TIME ) std::cerr << " (Time)";
  std::cerr << "\n";

  assert(false);
}

//---

CQPerfTraceData::
CQPerfTraceData(const QString &name) :
 name_(name)
{
  std::string pattern;

  CEnvInst.get("CQ_PERF_MONITOR_DEBUG_PATTERN", pattern);

  if (pattern.empty())
    pattern = "*";

  auto patterns = QString(pattern.c_str()).split("|", QString::SkipEmptyParts);

  for (int i = 0; i < patterns.length(); ++i) {
    QRegExp regexp(patterns[i], Qt::CaseSensitive, QRegExp::WildcardUnix);

    if (regexp.exactMatch(name)) {
      debug_ = true;
      break;
    }
  }
}

//---

void
CQPerfTraceData::
startTrace(int depth)
{
  timeData_.depth = depth;
  timeData_.start = CHRTime::getTime();
}

void
CQPerfTraceData::
endTrace(TraceType traceType)
{
  // calc elapsed time
  CHRTime endTime = CHRTime::getTime();

  timeData_.elapsed = CHRTime::diffTime(timeData_.start, endTime);

  addTrace(timeData_, traceType);
}

void
CQPerfTraceData::
addTrace(const TimeData &timeData, TraceType traceType)
{
  // update number of calls, total time, max and min time
  ++calls_;

  elapsed_ += timeData.elapsed;

  if (elapsedMin_.isSet()) {
    elapsedMin_ = std::min(elapsedMin_, timeData.elapsed);
    elapsedMax_ = std::max(elapsedMax_, timeData.elapsed);
  }
  else {
    elapsedMin_ = timeData.elapsed;
    elapsedMax_ = timeData.elapsed;
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
      times_.push_back(timeData);
    else {
      setTimeData(posEnd(), timeData);
    }
  }
  else
    times_.push_back(timeData);

  if (recording_) {
    if (traceType != TraceType::NO_RECORD)
      recordTimes_.push_back(timeData);
  }

  //---

  if (windowTime.isSet()) {
    int i = 0;
    int n = windowCount;

    for ( ; i < n; ++i) {
      int pos = fixPos(posStart_ + i);

      if (this->timeData(pos).start >= windowTime)
        break;
    }
  }

  //---

  if (maxCalls_ > 0 && calls_ > maxCalls_)
    CQPerfMonitorInst->alert(this, CQPerfMonitor::AlertType::CALLS);

  if (maxTime_.isSet() && elapsedMax_ > maxTime_)
    CQPerfMonitorInst->alert(this, CQPerfMonitor::AlertType::TIME);
}

//---

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

  addDebug(timeData_);
}

void
CQPerfTraceData::
addDebug(const TimeData &timeData)
{
  if (! isEnabled() || ! CQPerfMonitorInst->isEnabled()) {
    ++calls_;

    elapsed_ += timeData.elapsed;

    if (elapsedMin_.isSet()) {
      elapsedMin_ = std::min(elapsedMin_, timeData.elapsed);
      elapsedMax_ = std::max(elapsedMax_, timeData.elapsed);
    }
    else {
      elapsedMin_ = timeData.elapsed;
      elapsedMax_ = timeData.elapsed;
    }
  }
}

//---

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

//---

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

//---

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

//---

void
CQPerfTraceData::
reportStats()
{
  std::cerr << "Calls:    " << calls_      << "\n";
  std::cerr << "Min Time: " << elapsedMin_ << "\n";
  std::cerr << "Max Time: " << elapsedMax_ << "\n";
}

//---

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
