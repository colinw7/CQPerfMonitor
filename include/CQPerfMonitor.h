#ifndef CPerfMonitor_H
#define CPerfMonitor_H

#include <CHRTime.h>
#include <cassert>
#include <QObject>

#include <map>
#include <vector>
#include <iostream>

class CQPerfTraceData;

#define CQPerfMonitorInst CQPerfMonitor::getInstance()

class CQPerfMonitor : public QObject {
  Q_OBJECT

 public:
  static CQPerfMonitor *getInstance() {
    static CQPerfMonitor *inst;

    if (! inst)
      inst = new CQPerfMonitor;

    return inst;
  }

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b);

  bool isDebug() const { return debug_; }
  void setDebug(bool b);

  int windowCount() const { return windowCount_; }
  void setWindowCount(int i) { windowCount_ = i; }

  const CHRTime &windowTime() const { return windowTime_; }
  void setWindowTime(const CHRTime &v) { windowTime_ = v; }

  void startTrace(const QString &name);
  void endTrace  (const QString &name);

  void startDebug(const QString &name);
  void endDebug  (const QString &name);

  void resetTrace(const QString &name);
  void resetAll();

  bool isRecording() const { return recording_; }

  void startRecording();
  void stopRecording();

  bool isTraceEnabled(const QString &name);
  void setTraceEnabled(const QString &name, bool enabled);

  bool isTraceDebug(const QString &name);
  void setTraceDebug(const QString &name, bool debug);

  void setTraceMaxTime (const QString &name, const CHRTime &t);
  void setTraceMaxCalls(const QString &name, int n);

  void reportStats(const QString &name);

  CQPerfTraceData *getTrace(const QString &name);

  void getTraceNames(QStringList &names);

 signals:
  void stateChanged();

  void traceAdded(const QString &name);

 private:
  CQPerfMonitor();

 private:
  typedef std::map<QString,CQPerfTraceData *> Traces;

  bool    enabled_     { false };
  bool    debug_       { false };
  bool    recording_   { false };
  Traces  traces_;
  int     windowCount_ { 1000 };
  CHRTime windowTime_;
  int     numTrace_    { 0 };
  int     numDebug_    { 0 };
};


class CQPerfTraceData {
 public:
  struct WindowData {
    int     numCalls { 0 };
    CHRTime elapsed;
    CHRTime minT;
    CHRTime maxT;
  };

  using WindowDatas = std::vector<WindowData>;

  struct TimeData {
    int     depth { 0 };
    CHRTime start;
    CHRTime elapsed;
  };

  using TimeDatas = std::vector<TimeData>;

 public:
  CQPerfTraceData(const QString &name);

  const QString &name() const { return name_; }

  //---

  void startTrace(int depth=0);
  void endTrace  ();

  //---

  void startDebug(int depth=0);
  void endDebug  ();

  //---

  void reset();

  //---

  bool isRecording() const { return recording_; }

  void startRecording();
  void stopRecording();

  //---

  bool isEnabled() const { return enabled_; }
  void setEnabled(bool b) { enabled_ = b; }

  bool isDebug() const { return debug_; }
  void setDebug(bool b) { debug_ = b; }

  int numCalls() const { return calls_; }

  const CHRTime &elapsed() const { return elapsed_; }

  const CHRTime &elapsedMin() const { return elapsedMin_; }
  const CHRTime &elapsedMax() const { return elapsedMax_; }

  int maxCalls() const { return maxCalls_; }
  void setMaxCalls(int i) { maxCalls_ = i; }

  const CHRTime &maxTime() const { return maxTime_; }
  void setMaxTime(const CHRTime &v) { maxTime_ = v; }

  const CHRTime &elapsedTime() const { return timeData_.elapsed; }

  const CHRTime &windowStartTime() const;

  const CHRTime &windowEndTime() const;

  void windowDetails(WindowData &windowData) const;

  void windowDetails(const CHRTime &t1, const CHRTime &t2, WindowData &windowData) const;
  void windowDetails(const CHRTime &t1, const CHRTime &t2, TimeDatas &timeDatas) const;

  const TimeDatas &recordTimes() const { return recordTimes_; }

  void reportStats();

 private:
  int posEnd() const;

  int fixPos(int i) const;

  int windowSize() const;

 private:
  const TimeData &timeData(int i) const {
    assert(i >= 0 && i < int(times_.size()));

    return times_[i];
  }

  void setTimeData(int i, const TimeData &t) {
    assert(i >= 0 && i < int(times_.size()));

    times_[i] = t;
  }

 private:
  QString   name_;
  bool      enabled_      { true };
  bool      debug_        { false };
  bool      recording_    { false };
  TimeData  timeData_;
  TimeDatas times_;
  TimeDatas recordTimes_;
  int       posStart_     { 0 };
  int       posStartNext_ { 0 };
  int       sizeLimit_    { -1 };
  int       calls_        { 0 };
  CHRTime   elapsed_;
  CHRTime   elapsedMin_;
  CHRTime   elapsedMax_;
  int       maxCalls_     { -1 };
  CHRTime   maxTime_;
};

//------

class CQPerfTrace {
 public:
  CQPerfTrace(const QString &name) :
   name_(name) {
    if (CQPerfMonitorInst->isEnabled())
      CQPerfMonitorInst->startTrace(name_);

    if (CQPerfMonitorInst->isDebug())
      CQPerfMonitorInst->startDebug(name_);
  }

 ~CQPerfTrace() {
    if (CQPerfMonitorInst->isDebug())
      CQPerfMonitorInst->endDebug(name_);

    if (CQPerfMonitorInst->isEnabled())
      CQPerfMonitorInst->endTrace(name_);
  }

 private:
  QString name_;
};

#endif
