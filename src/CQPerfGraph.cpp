#include <CQPerfGraph.h>
#include <CQPerfMonitor.h>
#include <CInterval.h>

#include <CQImageButton.h>
#include <CQPixmapCache.h>

#include <QHeaderView>
#include <QSplitter>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QToolTip>

#include <svg/record_svg.h>
#include <svg/stop_svg.h>
#include <svg/zoom_out_svg.h>
#include <svg/zoom_in_svg.h>

namespace {

QColor bg_colors[] = {
  QColor(0x31,0x82,0xBD),
  QColor(0xE6,0x55,0x0D),
  QColor(0x31,0xA3,0x54),
  QColor(0x75,0x6B,0xB1),

  QColor(0x6B,0xAE,0xD6),
  QColor(0xFD,0x8D,0x3C),
  QColor(0x74,0xC4,0x76),
  QColor(0x9E,0x9A,0xC8),

  QColor(0x9E,0xCA,0xE1),
  QColor(0xFD,0xAE,0x6B),
  QColor(0xA1,0xD9,0x9B),
  QColor(0xBC,0xBD,0xDC),

  QColor(0xC6,0xDB,0xEF),
  QColor(0xFD,0xD0,0xA2),
  QColor(0xC7,0xE9,0xC0),
  QColor(0xDA,0xDA,0xEB),
};

int num_bg_colors = 16;

QColor bgColor(int i) {
  return bg_colors[i % num_bg_colors];
}

QString formatTime(double usecs) {
  if      (usecs >= 1000000)
    return QString().sprintf("%.3f secs", usecs/1000000);
  else if (usecs >= 1000)
    return QString().sprintf("%.3f msecs", usecs/1000);
  else
    return QString().sprintf("%.3f usecs", usecs);
}

QString formatTime(const CHRTime &t) {
  return formatTime(t.getUSecs());
}

};

CQPerfDialog *
CQPerfDialog::
instance()
{
  static CQPerfDialog *dlg;

  if (! dlg)
    dlg = new CQPerfDialog;

  return dlg;
}

CQPerfDialog::
CQPerfDialog(QWidget *parent) :
 QDialog(parent)
{
  setObjectName("perfDlg");

  setWindowTitle("Performance Monitor Graph");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  //----

  QFrame *controlFrame = new QFrame;
  controlFrame->setObjectName("controlFrame");

  controlFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  QHBoxLayout *controlLayout = new QHBoxLayout(controlFrame);
  controlLayout->setMargin(2); controlLayout->setSpacing(2);

  //---

  enableCheck_ = new QCheckBox("Enabled");
  enableCheck_->setObjectName("enableCheck");

  connect(enableCheck_, SIGNAL(stateChanged(int)), this, SLOT(enabledSlot(int)));

  debugCheck_ = new QCheckBox("Debug");
  debugCheck_->setObjectName("debugCheck");

  connect(debugCheck_, SIGNAL(stateChanged(int)), this, SLOT(debugSlot(int)));

  controlLayout->addWidget(enableCheck_);
  controlLayout->addWidget(debugCheck_);

  //---

  typeCombo_ = new QComboBox;
  typeCombo_->setObjectName("typeCombo");

  typeCombo_->addItems(QStringList() << "Totals" << "Intervals" << "Depth" << "Recording");

  controlLayout->addWidget(typeCombo_);

  //---

  shapeCombo_ = new QComboBox;
  shapeCombo_->setObjectName("shapeCombo");

  shapeCombo_->addItems(QStringList() << "Lines" << "Rects" << "Lines & Rects");

  controlLayout->addWidget(shapeCombo_);

  //---

  valueCombo_ = new QComboBox;
  valueCombo_->setObjectName("valueCombo");

  valueCombo_->addItems(QStringList() << "Elapsed" << "Count" << "Elapsed & Count");

  controlLayout->addWidget(valueCombo_);

  //---

  controlLayout->addWidget(new QLabel("Window Size"));

  windowSizeSpin_ = new QSpinBox();
  windowSizeSpin_->setObjectName("windowSizeSpin");

  windowSizeSpin_->setRange(1, 100000);

  connect(windowSizeSpin_, SIGNAL(valueChanged(int)), this, SLOT(windowSizeSlot(int)));

  controlLayout->addWidget(windowSizeSpin_);

  //---

  recordButton_ = new CQImageButton(CQPixmapCacheInst->getIcon("RECORD"));
  recordButton_->setObjectName("recordButton");

  if (CQPerfMonitorInst->isRecording())
    recordButton_->setIcon(CQPixmapCacheInst->getIcon("STOP"));

  connect(recordButton_, SIGNAL(clicked()), this, SLOT(recordSlot()));

  controlLayout->addWidget(recordButton_);

  //---

  controlLayout->addStretch(1);

  layout->addWidget(controlFrame);

  //----

  QSplitter *splitter = new QSplitter;

  splitter->setObjectName("splitter");
  splitter->setOrientation(Qt::Vertical);

  layout->addWidget(splitter);

  //----

  QFrame *graphFrame = new QFrame;
  graphFrame->setObjectName("graphFrame");

  QVBoxLayout *graphLayout = new QVBoxLayout(graphFrame);
  graphLayout->setMargin(0); graphLayout->setSpacing(0);

  splitter->addWidget(graphFrame);

  //--

  graph_ = new CQPerfGraph(this);

  splitter->addWidget(graph_);

  graphLayout->addWidget(graph_);

  //--

  QFrame *scrollFrame = new QFrame;
  scrollFrame->setObjectName("scrollFrame");

  scrollFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  graphLayout->addWidget(scrollFrame);

  //--

  QHBoxLayout *scrollLayout = new QHBoxLayout(scrollFrame);
  scrollLayout->setMargin(2); scrollLayout->setSpacing(2);

  CQImageButton *zoomOutButton = new CQImageButton(CQPixmapCacheInst->getIcon("ZOOM_OUT"));
  zoomOutButton->setObjectName("zoomOutButton");

  connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOutSlot()));

  scrollLayout->addWidget(zoomOutButton);

  graphScroll_ = new QScrollBar;
  graphScroll_->setObjectName("graphScroll");

  graphScroll_->setOrientation(Qt::Horizontal);
  graphScroll_->setRange(0, 0);

  connect(graphScroll_, SIGNAL(valueChanged(int)), this, SLOT(scrollSlot(int)));

  scrollLayout->addWidget(graphScroll_);

  CQImageButton *zoomInButton  = new CQImageButton(CQPixmapCacheInst->getIcon("ZOOM_IN"));
  zoomInButton->setObjectName("zoomInButton");

  connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomInSlot()));

  scrollLayout->addWidget(zoomInButton);

  int s = graphScroll_->sizeHint().height() + 4;

  zoomOutButton->setFixedSize(QSize(s, s));
  zoomInButton ->setFixedSize(QSize(s, s));

  //----

  list_ = new CQPerfList(this);

  splitter->addWidget(list_);

  connect(list_, SIGNAL(nameSelected(const QString &)),
          this, SLOT(setName(const QString &)));
  connect(list_, SIGNAL(namesSelected(const QStringList &)),
          this, SLOT(setNames(const QStringList &)));

  //----

  stateSlot();

  if      (graph_->isShowTotal())
    typeCombo_->setCurrentIndex(0);
  else if (graph_->isShowDepth())
    typeCombo_->setCurrentIndex(2);
  else if (graph_->isShowRecording())
    typeCombo_->setCurrentIndex(3);
  else
    typeCombo_->setCurrentIndex(1);

  if      (graph_->isShowPoints() && graph_->isShowRects())
    shapeCombo_->setCurrentIndex(2);
  else if (graph_->isShowPoints())
    shapeCombo_->setCurrentIndex(0);
  else if (graph_->isShowRects())
    shapeCombo_->setCurrentIndex(1);

  if      (graph_->isShowElapsed() && graph_->isShowCount())
    valueCombo_->setCurrentIndex(2);
  else if (graph_->isShowElapsed())
    valueCombo_->setCurrentIndex(0);
  else if (graph_->isShowCount())
    valueCombo_->setCurrentIndex(1);

  windowSizeSpin_->setValue(graph_->windowSize());

  //---

  connect(CQPerfMonitorInst, SIGNAL(stateChanged()), this, SLOT(stateSlot()));

  connect(typeCombo_ , SIGNAL(currentIndexChanged(int)), this, SLOT(typeComboSlot (int)));
  connect(shapeCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(shapeComboSlot(int)));
  connect(valueCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(valueComboSlot(int)));

  setTimeout(timeout_);
}

CQPerfDialog::
~CQPerfDialog()
{
  delete timer_;
}

void
CQPerfDialog::
setName(const QString &name)
{
  graph_->setName(name);
}

void
CQPerfDialog::
setNames(const QStringList &names)
{
  graph_->setNames(names);
}

void
CQPerfDialog::
setTimeout(int t)
{
  if (t > 0 && t != timeout_) {
    timeout_ = t;

    delete timer_;

    timer_ = 0;
  }

  if (! timer_) {
    timer_ = new QTimer;

    connect(timer_, SIGNAL(timeout()), this, SLOT(updateWidgets()));

    timer_->start(timeout_);
  }
}

void
CQPerfDialog::
enabledSlot(int state)
{
  CQPerfMonitorInst->setEnabled(state);
}

void
CQPerfDialog::
debugSlot(int state)
{
  CQPerfMonitorInst->setDebug(state);
}

void
CQPerfDialog::
typeComboSlot(int ind)
{
  if      (ind == 0) {
    graph_->setShowTotal    (true);
    graph_->setShowDepth    (false);
    graph_->setShowRecording(false);
  }
  else if (ind == 1) {
    graph_->setShowTotal    (false);
    graph_->setShowDepth    (false);
    graph_->setShowRecording(false);
  }
  else if (ind == 2) {
    graph_->setShowTotal    (true);
    graph_->setShowDepth    (true);
    graph_->setShowRecording(false);
  }
  else if (ind == 3) {
    graph_->setShowTotal    (false);
    graph_->setShowDepth    (false);
    graph_->setShowRecording(true);
  }
}

void
CQPerfDialog::
shapeComboSlot(int ind)
{
  if      (ind == 0) {
    graph_->setShowPoints(true);
    graph_->setShowRects (false);
  }
  else if (ind == 1) {
    graph_->setShowPoints(false);
    graph_->setShowRects (true);
  }
  else {
    graph_->setShowPoints(true);
    graph_->setShowRects (true);
  }
}

void
CQPerfDialog::
valueComboSlot(int ind)
{
  if      (ind == 0) {
    graph_->setShowElapsed(true);
    graph_->setShowCount  (false);
  }
  else if (ind == 1) {
    graph_->setShowElapsed(false);
    graph_->setShowCount  (true);
  }
  else {
    graph_->setShowElapsed(true);
    graph_->setShowCount  (true);
  }
}

void
CQPerfDialog::
windowSizeSlot(int n)
{
  graph_->setWindowSize(n);
}

void
CQPerfDialog::
recordSlot()
{
  if (! CQPerfMonitorInst->isRecording()) {
    CQPerfMonitorInst->startRecording();

    recordButton_->setIcon(CQPixmapCacheInst->getIcon("STOP"));
  }
  else {
    recordButton_->setIcon(CQPixmapCacheInst->getIcon("RECORD"));

    CQPerfMonitorInst->stopRecording();
  }
}

void
CQPerfDialog::
stateSlot()
{
  disconnect(enableCheck_, SIGNAL(stateChanged(int)), this, SLOT(enabledSlot(int)));
  disconnect(debugCheck_, SIGNAL(stateChanged(int)), this, SLOT(debugSlot(int)));

  enableCheck_->setChecked(CQPerfMonitorInst->isEnabled());
  debugCheck_ ->setChecked(CQPerfMonitorInst->isDebug  ());

  connect(enableCheck_, SIGNAL(stateChanged(int)), this, SLOT(enabledSlot(int)));
  connect(debugCheck_, SIGNAL(stateChanged(int)), this, SLOT(debugSlot(int)));
}

void
CQPerfDialog::
zoomOutSlot()
{
  graph_->zoomOut();

  graphScroll_->setPageStep(100);
  graphScroll_->setRange(0, 100*(graph_->zoomFactor() - 1));
}

void
CQPerfDialog::
zoomInSlot()
{
  graph_->zoomIn();

  graphScroll_->setPageStep(100);
  graphScroll_->setRange(0, 100*(graph_->zoomFactor() - 1));
}

void
CQPerfDialog::
scrollSlot(int i)
{
  graph_->setZoomOffset(i/100.0);

  graph_->update();
}

void
CQPerfDialog::
updateWidgets()
{
  graph_->update ();
  list_ ->refresh();
}

//---

CQPerfGraph::
CQPerfGraph(QWidget *parent, const QString &name) :
 QFrame(parent)
{
  setObjectName("graph");

  setName(name);

  setContextMenuPolicy(Qt::DefaultContextMenu);
}

CQPerfGraph::
~CQPerfGraph()
{
}

void
CQPerfGraph::
setName(const QString &name)
{
  if (name != "")
    names_ = (QStringList() << name);
  else
    names_.clear();
}

void
CQPerfGraph::
setNames(const QStringList &names)
{
  names_ = names;
}

void
CQPerfGraph::
contextMenuEvent(QContextMenuEvent *event)
{
  QMenu *menu = new QMenu;

  //---

  QAction *totalAction = new QAction("Total", menu);

  totalAction->setCheckable(true);
  totalAction->setChecked(isShowTotal());
  totalAction->setToolTip("Show running totals");

  menu->addAction(totalAction);

  connect(totalAction, SIGNAL(triggered(bool)), this, SLOT(setShowTotal(bool)));

  (void) menu->exec(event->globalPos());

  //---

  delete menu;
}

void
CQPerfGraph::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  //---

  // fill background
  QColor ocolor(200,200,200);

  QBrush obrush(ocolor);

  p.fillRect(rect(), obrush);

  //---

  if (! names_.length())
    return;

  //---

  QFont font1 = font();

  axisFont_ = font1; axisFont_.setPointSizeF(0.8*axisFont_.pointSizeF());

  //---

  if (CQPerfMonitorInst->isEnabled()) {
    if (! isShowDepth() && ! isShowRecording())
      drawIntervalGraph(&p);
    else
      drawDepthGraph(&p);
  }
  else {
    if (isShowRecording())
      drawDepthGraph(&p);
    else
      drawStatsGraph(&p);
  }
}

void
CQPerfGraph::
drawIntervalGraph(QPainter *p)
{
  CHRTime endTime = CHRTime::getTime();
  CHRTime startTime;

  startTime.setUSecs(endTime.getUSecs() - windowSize()*1000);

  //---

  xmin_ = startTime.getUSecs();
  xmax_ = endTime  .getUSecs();

  double dx = (xmax_ - xmin_)/zoomFactor_;

  xmin_ += zoomOffset()*dx;
  xmax_ = xmin_ + dx;

  //---

  int nb = numIntervals();

  double dt = (xmax_ - xmin_)/nb;

  //---

  // get max calls and elapsed
  int    maxCalls   = 0;
  double maxElapsed = 0;

  for (int i = 0; i < names_.length(); ++i) {
    CQPerfTraceData *trace = CQPerfMonitorInst->getTrace(names_[i]);

    if (isShowTotal()) {
      CQPerfTraceData::WindowData windowData;

      trace->windowDetails(startTime, endTime, windowData);

      maxCalls   = std::max(maxCalls  , windowData.numCalls);
      maxElapsed = std::max(maxElapsed, windowData.elapsed.getUSecs());
    }
    else {
      for (int j = 0; j < nb; ++j) {
        // get step time range
        double tt1 = xmin_ + j*dt;
        double tt2 = tt1 + dt;

        CHRTime stepStartTime; stepStartTime.setUSecs(tt1);
        CHRTime stepEndTime  ; stepEndTime  .setUSecs(tt2);

        //---

        CQPerfTraceData::WindowData windowData;

        // get number of calls and elapsed for step time range
        trace->windowDetails(stepStartTime, stepEndTime, windowData);

        maxCalls   = std::max(maxCalls  , windowData.numCalls);
        maxElapsed = std::max(maxElapsed, windowData.elapsed.getUSecs());
      }
    }
  }

  //---

  // calc y axis ranges
  CInterval callsInterval, elapsedInterval;

  callsInterval = CInterval(0, std::max(maxCalls, 1), 10);

  callsInterval.setIntegral(true);

  ymin1_ = callsInterval.calcStart();
  ymax1_ = callsInterval.calcEnd  ();

  elapsedInterval = CInterval(0, std::max(maxElapsed, 1.0), 10);

  ymin2_ = elapsedInterval.calcStart();
  ymax2_ = elapsedInterval.calcEnd  ();

  //---

  // set margins
  QFontMetrics fm(font());

  int fa = fm.ascent();
//int fd = fm.descent();

  if      (isShowElapsed() && isShowCount()) {
    lmargin_ = fm.width(QString().sprintf("%d"  , maxCalls  )) + 8;
    rmargin_ = fm.width(QString().sprintf("%.3f", maxElapsed)) + 8;
  }
  else if (isShowElapsed()) {
    lmargin_ = fm.width(QString().sprintf("%.3f", maxElapsed)) + 8;
    rmargin_ = 2;
  }
  else if (isShowCount()) {
    lmargin_ = fm.width(QString().sprintf("%d"  , maxCalls  )) + 8;
    rmargin_ = 0;
  }
  else {
    lmargin_ = 2;
    rmargin_ = 2;
  }

  tmargin_ = fm.height() + fm.height()/2 + 4;
  bmargin_ = fm.height() + fm.height()/2 + 2;

  //---

  // fill plot background
  QRect irect(lmargin_, tmargin_, width() - lmargin_ - rmargin_,
              height() - tmargin_ - bmargin_);

  QBrush ibrush(Qt::white);

  p->fillRect(irect, ibrush);

  //---

  // draw axes
  if      (isShowElapsed() && isShowCount()) {
    drawAxis(p, callsInterval  , /*isLeft*/true , /*isCalls*/true );
    drawAxis(p, elapsedInterval, /*isLeft*/false, /*isCalls*/false);
  }
  else if (isShowElapsed()) {
    drawAxis(p, elapsedInterval, /*isLeft*/true, /*isCalls*/false);
  }
  else if (isShowCount()) {
    drawAxis(p, callsInterval, /*isLeft*/true, /*isCalls*/true);
  }

  // draw grid lines
  drawGrid(p, callsInterval);

  //---

  p->setPen(Qt::black);

  p->setFont(font());

  // draw title
  QString title;

  for (int i = 0; i < names_.length(); ++i) {
    if (i > 0)
      title += ", ";

    title += names_[i];
  }

  double tx = width()/2 - fm.width(title)/2;

  p->drawText(tx, fa + 2, title);

  //---

  // draw x label
  QString xlabel = formatTime(xmax_ - xmin_);

  tx = width()/2 - fm.width(xlabel)/2;

  p->drawText(tx, height() - fm.descent() - 2, xlabel);

  //---

  struct TraceDrawData {
    std::vector<QPointF> points1, points2;
    std::vector<QRectF>  rects1, rects2;
  };

  using TraceDrawDatas = std::vector<TraceDrawData>;

  TraceDrawDatas traceDrawDatas;

  traceDrawDatas.resize(names_.size());

  //---

  for (int i = 0; i < names_.length(); ++i) {
    CQPerfTraceData *trace = CQPerfMonitorInst->getTrace(names_[i]);

    //---

    TraceDrawData &traceDrawData = traceDrawDatas[i];

    if (isShowPoints()) {
      traceDrawData.points1.resize(nb);
      traceDrawData.points2.resize(nb);
    }

    if (isShowRects()) {
      traceDrawData.rects1.resize(nb);
      traceDrawData.rects2.resize(nb);
    }

    //---

    for (int j = 0; j < nb; ++j) {
      // get step time range
      double tt1 = xmin_ + j*dt;
      double tt2 = tt1 + dt;

      CHRTime stepStartTime; stepStartTime.setUSecs(tt1);
      CHRTime stepEndTime  ; stepEndTime  .setUSecs(tt2);

      //---

      CQPerfTraceData::WindowData windowData;

      if (isShowTotal()) {
        // get number of calls and elapsed from start of window to delta time
        trace->windowDetails(startTime, stepEndTime, windowData);
      }
      else {
        // get number of calls and elapsed for step time range
        trace->windowDetails(stepStartTime, stepEndTime, windowData);
      }

      //---

      // add points at mid point of step time range
      if (isShowPoints()) {
        double tt = (tt1 + tt2)/2.0;

        traceDrawData.points1[j] = QPointF(tt, windowData.numCalls          );
        traceDrawData.points2[j] = QPointF(tt, windowData.elapsed.getUSecs());
      }

      //---

      // add rects for time range
      if (isShowRects()) {
        traceDrawData.rects1[j] = QRectF(tt1, 0, tt2 - tt1, windowData.numCalls);
        traceDrawData.rects2[j] = QRectF(tt1, 0, tt2 - tt1, windowData.elapsed.getUSecs());
      }
    }
  }

  //---

  p->setClipRect(irect);

  for (int i = 0; i < names_.length(); ++i) {
    TraceDrawData &traceDrawData = traceDrawDatas[i];

    // draw rects
    if (isShowRects()) {
      //p->setPen(Qt::NoPen);
      QColor pc = QColor("#000000"); pc.setAlphaF(0.5);
      p->setPen(pc);

      if (isShowCount()) {
        for (const auto &r1 : traceDrawData.rects1) {
          double px1, py1, px2, py2;

          countToPixel(r1.left (), r1.bottom(), px1, py1);
          countToPixel(r1.right(), r1.top   (), px2, py2);

          QColor c1 = bgColor(i); c1.setAlphaF(0.5);
          p->setBrush(c1);

          p->drawRect(QRectF(px1, py1, px2 - px1, py2 - py1));
        }
      }

      if (isShowElapsed()) {
        for (const auto &r2 : traceDrawData.rects2) {
          double px1, py1, px2, py2;

          elapsedToPixel(r2.left (), r2.bottom(), px1, py1);
          elapsedToPixel(r2.right(), r2.top   (), px2, py2);

          QColor c2 = bgColor(i + 8); c2.setAlphaF(0.5);
          p->setBrush(c2);

          p->drawRect(QRectF(px1, py1, px2 - px1, py2 - py1));
        }
      }
    }

    //---

    // draw points as path
    if (isShowPoints()) {
      if (isShowCount()) {
        QPainterPath path1;

        int i1 = 0;

        for (const auto &p1 : traceDrawData.points1) {
          double px, py1;

          countToPixel(p1.x(), p1.y(), px, py1);

          if (i1 == 0)
            path1.moveTo(px, py1);
          else
            path1.lineTo(px, py1);

          ++i1;
        }

        QColor c1 = bgColor(i);

        QPen pen1(c1);

        p->strokePath(path1, pen1);
      }

      //---

      if (isShowElapsed()) {
        QPainterPath path2;

        int i2 = 0;

        for (const auto &p2 : traceDrawData.points2) {
          double px, py2;

          elapsedToPixel(p2.x(), p2.y(), px, py2);

          if (i2 == 0)
            path2.moveTo(px, py2);
          else
            path2.lineTo(px, py2);

          ++i2;
        }

        QColor c2 = bgColor(i + 8);

        QPen pen2(c2);

        p->strokePath(path2, pen2);
      }
    }
  }
}

void
CQPerfGraph::
drawDepthGraph(QPainter *p)
{
  CHRTime minTime, maxTime;

  if (isShowDepth()) {
    maxTime = CHRTime::getTime();

    minTime.setUSecs(maxTime.getUSecs() - windowSize()*1000);
  }
  else {
    minTime = CHRTime::getTime();
    maxTime = 0;
  }

  //---

  // get max depth
  int maxDepth = 0;

  for (int i = 0; i < names_.length(); ++i) {
    CQPerfTraceData *trace = CQPerfMonitorInst->getTrace(names_[i]);

    if (isShowDepth()) {
      CQPerfTraceData::TimeDatas timeDatas;

      trace->windowDetails(minTime, maxTime, timeDatas);

      for (const auto &timeData : timeDatas)
        maxDepth = std::max(maxDepth, timeData.depth);
    }
    else {
      const CQPerfTraceData::TimeDatas &timeDatas = trace->recordTimes();

      for (const auto &timeData : timeDatas) {
        maxDepth = std::max(maxDepth, timeData.depth);
        minTime  = std::min(minTime , timeData.start);
        maxTime  = std::max(maxTime , timeData.start + timeData.elapsed);
      }
    }
  }

  //---

  xmin_ = minTime.getUSecs();
  xmax_ = maxTime.getUSecs();

  double dx = (xmax_ - xmin_)/zoomFactor_;

  xmin_ += zoomOffset()*dx;
  xmax_ = xmin_ + dx;

  //---

  // calc y axis ranges
  CInterval depthInterval;

  depthInterval = CInterval(0, std::max(maxDepth, 1), 10);

  depthInterval.setIntegral(true);

  ymin1_ = depthInterval.calcStart();
  ymax1_ = depthInterval.calcEnd  ();

  //---

  // set margins
  QFontMetrics fm(font());

  int fa = fm.ascent();
  int fd = fm.descent();

  lmargin_ = fm.width(QString().sprintf("%d", maxDepth)) + 8;
  rmargin_ = 0;

  tmargin_ = fm.height() + fm.height()/2 + 4;
  bmargin_ = fm.height() + fm.height()/2 + 2;

  //---

  // fill plot background
  QRect irect(lmargin_, tmargin_, width() - lmargin_ - rmargin_,
              height() - tmargin_ - bmargin_);

  QBrush ibrush(Qt::white);

  p->fillRect(irect, ibrush);

  //---

  // draw axes
  drawAxis(p, depthInterval, /*isLeft*/true, /*isCalls*/true);

  // draw grid lines
  drawGrid(p, depthInterval);

  //---

  p->setPen(Qt::black);

  p->setFont(font());

  // draw title
  QString title;

  for (int i = 0; i < names_.length(); ++i) {
    if (i > 0)
      title += ", ";

    title += names_[i];
  }

  double tx = width()/2 - fm.width(title)/2;

  p->drawText(tx, fa + 2, title);

  //---

  // draw x label
  QString xlabel = formatTime(xmax_ - xmin_);

  tx = width()/2 - fm.width(xlabel)/2;

  p->drawText(tx, height() - fm.descent() - 2, xlabel);

  //---

  struct RectTip {
    QRectF  rect;
    QString name;
    QString tip;

    RectTip(const QRectF &rect, const QString &name, const QString &tip) :
     rect(rect), name(name), tip(tip) {
    }
  };

  struct TraceRectTips {
    std::vector<RectTip> rectTips;
  };

  using TraceDrawDatas = std::vector<TraceRectTips>;

  TraceDrawDatas traceDrawDatas;

  traceDrawDatas.resize(names_.size());

  tipRects_.clear();

  //---

  for (int i = 0; i < names_.length(); ++i) {
    CQPerfTraceData *trace = CQPerfMonitorInst->getTrace(names_[i]);

    TraceRectTips &traceRectTips = traceDrawDatas[i];

    //---

    if (isShowDepth()) {
      CQPerfTraceData::TimeDatas timeDatas;

      trace->windowDetails(minTime, maxTime, timeDatas);

      for (const auto &timeData : timeDatas) {
        double startTime = timeData.start.getUSecs();
        double deltaTime = timeData.elapsed.getUSecs();

        QString tipText =
          QString("<table>"
                  "<tr><td colspan=2>%1</td></tr>"
                  "<tr><td>Elapsed</td><td>%2</td></tr>"
                  "</table>").
                  arg(names_[i]).
                  arg(formatTime(deltaTime));

        QRectF rect(startTime, timeData.depth - 1, deltaTime, 1);

        traceRectTips.rectTips.push_back(RectTip(rect, names_[i], tipText));
      }
    }
    else {
      const CQPerfTraceData::TimeDatas &timeDatas = trace->recordTimes();

      for (const auto &timeData : timeDatas) {
        double startTime = timeData.start.getUSecs();
        double deltaTime = timeData.elapsed.getUSecs();

        QString tipText =
          QString("<table>"
                  "<tr><td colspan=2>%1</td></tr>"
                  "<tr><td>Elapsed</td><td>%2</td></tr>"
                  "</table>").
                  arg(names_[i]).
                  arg(formatTime(deltaTime));

        QRectF rect(startTime, timeData.depth - 1, deltaTime, 1);

        traceRectTips.rectTips.push_back(RectTip(rect, names_[i], tipText));
      }
    }
  }

  //---

  p->setFont(axisFont_);

  p->setClipRect(irect);

  for (int i = 0; i < names_.length(); ++i) {
    TraceRectTips &traceRectTips = traceDrawDatas[i];

    // draw rects
    for (const auto &rectTip : traceRectTips.rectTips) {
      const QRectF &r = rectTip.rect;

      double px1, py1, px2, py2;

      countToPixel(r.left (), r.bottom(), px1, py1);
      countToPixel(r.right(), r.top   (), px2, py2);

      QColor pc = QColor("#000000"); pc.setAlphaF(0.5);
      p->setPen(pc);

      QColor c = bgColor(i);
      p->setBrush(c);

      QRectF rect(px1, py1, px2 - px1, py2 - py1);

      p->drawRect(rect);

      p->setPen(Qt::black);

      p->setClipRect(rect.adjusted(2, 2, -2, -2));

      p->drawText(rect.left() + 2, rect.center().y() + (fa - fd)/2, rectTip.name);

      p->setClipRect(irect);

      tipRects_.push_back(TipRect(rect, rectTip.tip));
    }
  }
}

void
CQPerfGraph::
drawStatsGraph(QPainter *p)
{
  xmin_ = 0;
  xmax_ = names_.length();

  double dx = (xmax_ - xmin_)/zoomFactor_;

  xmin_ += zoomOffset()*dx;
  xmax_ = xmin_ + dx;

  //---

  // get max calls and elapsed
  int    maxCalls   = 0;
  double maxElapsed = 0;

  for (int i = 0; i < names_.length(); ++i) {
    CQPerfTraceData *trace = CQPerfMonitorInst->getTrace(names_[i]);

    maxCalls   = std::max(maxCalls  , trace->numCalls());
    maxElapsed = std::max(maxElapsed, trace->elapsedMax().getMSecs());
  }

  //---

  // calc y axis ranges
  CInterval callsInterval, elapsedInterval;

  callsInterval = CInterval(0, std::max(maxCalls, 1), 10);

  callsInterval.setIntegral(true);

  ymin1_ = callsInterval.calcStart();
  ymax1_ = callsInterval.calcEnd  ();

  elapsedInterval = CInterval(0, std::max(maxElapsed, 1.0), 10);

  ymin2_ = elapsedInterval.calcStart();
  ymax2_ = elapsedInterval.calcEnd  ();

  //---

  // set margins
  QFontMetrics fm(font());

  int fa = fm.ascent();
//int fd = fm.descent();

  if      (isShowElapsed() && isShowCount()) {
    lmargin_ = fm.width(QString().sprintf("%d"  , maxCalls  )) + 8;
    rmargin_ = fm.width(QString().sprintf("%.3f", maxElapsed)) + 8;
  }
  else if (isShowElapsed()) {
    lmargin_ = fm.width(QString().sprintf("%.3f", maxElapsed)) + 8;
    rmargin_ = 2;
  }
  else if (isShowCount()) {
    lmargin_ = fm.width(QString().sprintf("%d"  , maxCalls  )) + 8;
    rmargin_ = 0;
  }
  else {
    lmargin_ = 2;
    rmargin_ = 2;
  }

  tmargin_ = fm.height() + fm.height()/2 + 4;
  bmargin_ = fm.height() + fm.height()/2 + 2;

  //---

  // fill plot background
  QRect irect(lmargin_, tmargin_, width() - lmargin_ - rmargin_,
              height() - tmargin_ - bmargin_);

  QBrush ibrush(Qt::white);

  p->fillRect(irect, ibrush);

  //---

  // draw axes
  if      (isShowElapsed() && isShowCount()) {
    drawAxis(p, callsInterval  , /*isLeft*/true , /*isCalls*/true );
    drawAxis(p, elapsedInterval, /*isLeft*/false, /*isCalls*/false);
  }
  else if (isShowElapsed()) {
    drawAxis(p, elapsedInterval, /*isLeft*/true, /*isCalls*/false);
  }
  else if (isShowCount()) {
    drawAxis(p, callsInterval, /*isLeft*/true, /*isCalls*/true);
  }

  // draw grid lines
  drawGrid(p, callsInterval);

  //---

  p->setPen(Qt::black);

  p->setFont(font());

  // draw title
  QString title;

  for (int i = 0; i < names_.length(); ++i) {
    if (i > 0)
      title += ", ";

    title += names_[i];
  }

  double tx = width()/2 - fm.width(title)/2;

  p->drawText(tx, fa + 2, title);

  //---

  // draw x label
  QString xlabel = formatTime(xmax_ - xmin_);

  tx = width()/2 - fm.width(xlabel)/2;

  p->drawText(tx, height() - fm.descent() - 2, xlabel);

  //---

  tipRects_.clear();

  p->setPen(Qt::black);

  p->setClipRect(irect);

  // draw counts
  for (int i = 0; i < names_.length(); ++i) {
    CQPerfTraceData *trace = CQPerfMonitorInst->getTrace(names_[i]);

    QString tipText =
      QString("<table>"
              "<tr><td colspan=2>%1</td></tr>"
              "<tr><td>Count</td><td>%2</td></tr>"
              "<tr><td>Min</td><td>%3</td></tr>"
              "<tr><td>Max</td><td>%4</td></tr>"
              "</table>").
              arg(names_[i]).
              arg(trace->numCalls()).
              arg(formatTime(trace->elapsedMin())).
              arg(formatTime(trace->elapsedMax()));

    if      (isShowElapsed() && isShowCount()) {
      double px1, py1, px2, py2;

      countToPixel(i + 0.0, 0                , px1, py1);
      countToPixel(i + 0.5, trace->numCalls(), px2, py2);

      QRectF rect1(px1, py1, px2 - px1, py2 - py1);

      QColor c1 = bgColor(i);
      p->setBrush(c1);

      p->drawRect(rect1);

      tipRects_.push_back(TipRect(rect1, tipText));

      //---

      elapsedToPixel(i + 0.5, trace->elapsedMin().getMSecs(), px1, py1);
      elapsedToPixel(i + 1.0, trace->elapsedMax().getMSecs(), px2, py2);

      QRectF rect2(px1, py1, px2 - px1, py2 - py1);

      QColor c2 = bgColor(i + 8);
      p->setBrush(c2);

      p->drawRect(rect2);

      tipRects_.push_back(TipRect(rect2, tipText));
    }
    else if (isShowElapsed()) {
      double px1, py1, px2, py2;

      elapsedToPixel(i + 0.0, trace->elapsedMin().getMSecs(), px1, py1);
      elapsedToPixel(i + 1.0, trace->elapsedMax().getMSecs(), px2, py2);

      QRectF rect(px1, py1, px2 - px1, py2 - py1);

      QColor c = bgColor(i);
      p->setBrush(c);

      p->drawRect(rect);

      tipRects_.push_back(TipRect(rect, tipText));
    }
    else if (isShowCount()) {
      double px1, py1, px2, py2;

      countToPixel(i + 0.0, 0                , px1, py1);
      countToPixel(i + 1.0, trace->numCalls(), px2, py2);

      QRectF rect(px1, py1, px2 - px1, py2 - py1);

      QColor c = bgColor(i);
      p->setBrush(c);

      p->drawRect(rect);

      tipRects_.push_back(TipRect(rect, tipText));
    }
  }
}

void
CQPerfGraph::
drawAxis(QPainter *p, const CInterval &interval, bool isLeft, bool isCalls)
{
  QPen pen3(Qt::black);

  p->setPen (pen3);
  p->setFont(axisFont_);

  QFontMetrics fm(axisFont_);

  int fa = fm.ascent();
  int fd = fm.descent();

  //---

  double ay1 = height() - bmargin_;
  double ay2 = tmargin_;

  double dy1 = (ay1 - ay2)/interval.calcNumMajor();

  for (int i = 0; i <= interval.calcNumMajor(); ++i) {
    double y = interval.interval(i);

    double ay = ay1 - i*dy1;

    QString text;

    if (isCalls)
      text = QString("%1").arg(y);
    else
      text = QString().sprintf("%.3f", y/1000.0);

    double ax;

    if (isLeft)
      ax = lmargin_ - fm.width(text) - 6;
    else
      ax = width() - rmargin_ + 6;

    p->drawText(ax, ay + (fa - fd)/2, text);

    if (isLeft)
      p->drawLine(lmargin_ - 4, ay, lmargin_, ay);
    else
      p->drawLine(width() - rmargin_, ay, width() - rmargin_ + 4, ay);
  }

  if (isLeft)
    p->drawLine(lmargin_, ay1, lmargin_, ay2);
  else
    p->drawLine(width() - rmargin_, ay1, width() - rmargin_, ay2);
}

void
CQPerfGraph::
drawGrid(QPainter *p, const CInterval &interval)
{
  // draw grid lines
  QPen pen4(QColor("#aaaaaa"));

  p->setPen(pen4);

  double ay1 = height() - bmargin_;
  double ay2 = tmargin_;

  double dy1 = (ay1 - ay2)/interval.calcNumMajor();

  for (int i = 0; i <= interval.calcNumMajor(); ++i) {
    double ay = ay1 - i*dy1;

    double ax1 = lmargin_;
    double ax2 = width() - rmargin_;

    p->drawLine(ax1, ay, ax2, ay);
  }
}

void
CQPerfGraph::
zoomOut()
{
  if (zoomFactor_ > 1)
    --zoomFactor_;
}

void
CQPerfGraph::
zoomIn()
{
  if (zoomFactor_ < 16)
    ++zoomFactor_;
}

bool
CQPerfGraph::
event(QEvent *e)
{
  if (e->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);

    for (const auto &tipRect : tipRects_) {
      if (tipRect.rect.contains(helpEvent->pos())) {
        QToolTip::showText(helpEvent->globalPos(), tipRect.tip, this, tipRect.rect.toRect());
        return true;
      }
    }

    QToolTip::hideText();

    e->ignore();

    return true;
  }

  return QWidget::event(e);
}

void
CQPerfGraph::
countToPixel(double x, double y, double &px, double &py)
{
  double w = std::max(1, width () - lmargin_ - rmargin_ - 1);
  double h = std::max(1, height() - tmargin_ - bmargin_ - 1);

  px = ((x - xmin_)/(xmax_ - xmin_))*w + lmargin_;

  if (ymax1_ != ymin1_)
    py = ((y - ymax1_)/(ymin1_ - ymax1_))*h + tmargin_;
  else
    py = height() - bmargin_;
}

void
CQPerfGraph::
elapsedToPixel(double x, double y, double &px, double &py)
{
  double w = std::max(1, width () - lmargin_ - rmargin_ - 1);
  double h = std::max(1, height() - tmargin_ - bmargin_ - 1);

  px = ((x - xmin_)/(xmax_ - xmin_))*w + lmargin_;

  if (ymax2_ != ymin2_)
    py = ((y - ymax2_)/(ymin2_ - ymax2_))*h + tmargin_;
  else
    py = height() - bmargin_;
}

//---

CQPerfList::
CQPerfList(QWidget *parent) :
 QTableWidget(parent)
{
  setObjectName("list");

  if (isSingleSelect())
    setSelectionMode(QAbstractItemView::SingleSelection);
  else
    setSelectionMode(QAbstractItemView::ExtendedSelection);

  setSelectionBehavior(QAbstractItemView::SelectRows);

  horizontalHeader()->setStretchLastSection(true);

  reload();

  connect(CQPerfMonitorInst, SIGNAL(traceAdded(const QString &)), this, SLOT(reload()));

  connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChangeSlot()));

  connect(this, SIGNAL(cellClicked(int, int)), this, SLOT(clickSlot(int, int)));
}

void
CQPerfList::
reload()
{
  QStringList names;

  CQPerfMonitorInst->getTraceNames(names);

  clear();

  setColumnCount(7);
  setRowCount(names.length());

  setHorizontalHeaderItem(0, new QTableWidgetItem("Enabled"    ));
  setHorizontalHeaderItem(1, new QTableWidgetItem("Debug"      ));
  setHorizontalHeaderItem(2, new QTableWidgetItem("Name"       ));
  setHorizontalHeaderItem(3, new QTableWidgetItem("Count"      ));
  setHorizontalHeaderItem(4, new QTableWidgetItem("Elapsed"    ));
  setHorizontalHeaderItem(5, new QTableWidgetItem("Min Elapsed"));
  setHorizontalHeaderItem(6, new QTableWidgetItem("Max Elapsed"));

  for (int i = 0; i < names.length(); ++i) {
    QTableWidgetItem *enabledItem = new QTableWidgetItem("");
    QTableWidgetItem *debugItem   = new QTableWidgetItem("");
    QTableWidgetItem *nameItem    = new QTableWidgetItem(names[i]);
    QTableWidgetItem *countItem   = new QTableWidgetItem("");
    QTableWidgetItem *elapsedItem = new QTableWidgetItem("");
    QTableWidgetItem *minItem     = new QTableWidgetItem("");
    QTableWidgetItem *maxItem     = new QTableWidgetItem("");

    enabledItem->setCheckState(Qt::Unchecked);
    debugItem  ->setCheckState(Qt::Unchecked);
    elapsedItem->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    minItem    ->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    maxItem    ->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

    setItem(i, 0, enabledItem);
    setItem(i, 1, debugItem  );
    setItem(i, 2, nameItem   );
    setItem(i, 3, countItem  );
    setItem(i, 4, elapsedItem);
    setItem(i, 5, minItem    );
    setItem(i, 6, maxItem    );
  }
}

void
CQPerfList::
refresh()
{
  for (int i = 0; i < rowCount(); ++i) {
    QTableWidgetItem *enabledItem = item(i, 0);
    QTableWidgetItem *debugItem   = item(i, 1);
    QTableWidgetItem *nameItem    = item(i, 2);
    QTableWidgetItem *countItem   = item(i, 3);
    QTableWidgetItem *elapsedItem = item(i, 4);
    QTableWidgetItem *minItem     = item(i, 5);
    QTableWidgetItem *maxItem     = item(i, 6);

    QString name = nameItem->text();

    CQPerfTraceData *data = CQPerfMonitorInst->getTrace(name);

    enabledItem->setCheckState(data->isEnabled() ? Qt::Checked : Qt::Unchecked);
    debugItem  ->setCheckState(data->isDebug  () ? Qt::Checked : Qt::Unchecked);

    countItem  ->setText(QString("%1").arg(data->numCalls()));
    elapsedItem->setText(QString().sprintf("%.6f", data->elapsed   ().getSecs ()));
    minItem    ->setText(QString().sprintf("%.6f", data->elapsedMin().getMSecs()));
    maxItem    ->setText(QString().sprintf("%.6f", data->elapsedMax().getMSecs()));

    nameItem   ->setToolTip(nameItem   ->text());
    countItem  ->setToolTip(countItem  ->text());
    elapsedItem->setToolTip(elapsedItem->text());
    minItem    ->setToolTip(minItem    ->text());
    maxItem    ->setToolTip(maxItem    ->text());
  }
}

void
CQPerfList::
selectionChangeSlot()
{
  if (isSingleSelect()) {
    for (int i = 0; i < rowCount(); ++i) {
      QTableWidgetItem *nameItem = item(i, 2);

      if (nameItem->isSelected()) {
        QString name = nameItem->text();

        emit nameSelected(name);

        return;
      }
    }
  }
  else {
    QStringList names;

    for (int i = 0; i < rowCount(); ++i) {
      QTableWidgetItem *nameItem = item(i, 2);

      if (nameItem->isSelected()) {
        QString name = nameItem->text();

        names.push_back(name);
      }
    }

    emit namesSelected(names);
  }
}

void
CQPerfList::
clickSlot(int row, int column)
{
  QTableWidgetItem *nameItem = item(row, 2);

  QString name = nameItem->text();

  CQPerfTraceData *data = CQPerfMonitorInst->getTrace(name);

  if      (column == 0) {
    QTableWidgetItem *enabledItem = item(row, 0);

    data->setEnabled(enabledItem->checkState() == Qt::Checked);
  }
  else if (column == 1) {
    QTableWidgetItem *debugItem = item(row, 1);

    data->setDebug(debugItem->checkState() == Qt::Checked);
  }
}
