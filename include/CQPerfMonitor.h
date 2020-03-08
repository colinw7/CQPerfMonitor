#ifndef CPerfMonitor_H
#define CPerfMonitor_H

#include <CHRTime.h>
#include <cassert>
#include <QObject>

#include <map>
#include <vector>
#include <future>
#include <iostream>

class CQPerfTraceData;

#define CQPerfMonitorInst CQPerfMonitor::getInstance()

/*!
 * \brief Class to collect statistics for elapsed time and number of calls of code block
 */
class CQPerfMonitor : public QObject {
  Q_OBJECT

 public:
  enum class TraceType {
    ALL,
    NO_RECORD
  };

  enum class AlertType {
    CALLS,
    TIME
  };

  using TraceList = std::vector<CQPerfTraceData *>;

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

  void startTrace(const QString &name, TraceType traceType=TraceType::ALL);
  void endTrace  (const QString &name, TraceType traceType=TraceType::ALL);

  void startDebug(const QString &name);
  void endDebug  (const QString &name);

  void resetTrace(const QString &name);
  void resetStartsWith(const QString &name);
  void resetAll();

  bool isRecording() const { return recording_; }

  void startRecording();
  void stopRecording();

  bool isTraceEnabled(const QString &name) const;
  void setTraceEnabled(const QString &name, bool enabled);

  bool isTraceDebug(const QString &name) const;
  void setTraceDebug(const QString &name, bool debug);

  void setTraceMaxTime (const QString &name, const CHRTime &t);
  void setTraceMaxCalls(const QString &name, int n);

  void reportStats(const QString &name) const;

  CQPerfTraceData *getTrace(const QString &name);

  void getTracesStartingWith(const QString &name, TraceList &traces);

  void getTraceNames(QStringList &names) const;

  void alert(const CQPerfTraceData *trace, CQPerfMonitor::AlertType type);

 signals:
  void stateChanged();

  void traceAdded(const QString &name);

 private:
  CQPerfMonitor();

 private:
  using Traces = std::map<QString,CQPerfTraceData *>;

  bool               enabled_     { false }; //!< is trace enabled
  bool               debug_       { false }; //!< is debug enabled
  bool               recording_   { false }; //!< is recording
  Traces             traces_;                //!< active traces
  int                windowCount_ { 1000 };  //!< number of traces to keep in history
  CHRTime            windowTime_;            //!< time span for history
  int                numTrace_    { 0 };     //!< number of active traces
  int                numDebug_    { 0 };     //!< number of active debugs
  mutable std::mutex mutex_;                 //!< update mutex
};

//---

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
  void endTrace  (CQPerfMonitor::TraceType traceType);

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

  int sizeLimit() const { return sizeLimit_; }

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
  QString   name_;                   //<! trace name
  bool      enabled_      { true };  //<! is enabled
  bool      debug_        { false }; //<! is debug enabled
  bool      recording_    { false }; //<! is recording
  TimeData  timeData_;               //<! time data
  TimeDatas times_;                  //<! history times
  TimeDatas recordTimes_;            //<! recorded times
  int       posStart_     { 0 };     //<! window start
  int       posStartNext_ { 0 };     //<! window next start
  int       sizeLimit_    { -1 };    //<! window size limit
  int       calls_        { 0 };     //<! number of calls
  CHRTime   elapsed_;                //<! last elapsed time
  CHRTime   elapsedMin_;             //<! min elapsed time
  CHRTime   elapsedMax_;             //<! max elapsed time
  int       maxCalls_     { -1 };    //<! max calls before alert
  CHRTime   maxTime_;                //<! max elapsed time before alert
};

//------

class CQPerfTrace {
 public:
  CQPerfTrace(const QString &name,
              CQPerfMonitor::TraceType traceType=CQPerfMonitor::TraceType::ALL) :
   name_(name), traceType_(traceType) {
    if (CQPerfMonitorInst->isEnabled())
      CQPerfMonitorInst->startTrace(name_, traceType_);

    if (CQPerfMonitorInst->isDebug())
      CQPerfMonitorInst->startDebug(name_);
  }

 ~CQPerfTrace() {
    if (CQPerfMonitorInst->isDebug())
      CQPerfMonitorInst->endDebug(name_);

    if (CQPerfMonitorInst->isEnabled())
      CQPerfMonitorInst->endTrace(name_, traceType_);
  }

 private:
  QString                  name_;
  CQPerfMonitor::TraceType traceType_ { CQPerfMonitor::TraceType::ALL };
};

#endif
