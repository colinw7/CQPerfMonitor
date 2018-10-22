#ifndef CQPerfGraph_H
#define CQPerfGraph_H

#include <QDialog>
#include <QFrame>
#include <QTableWidget>

class CQPerfGraph;
class CQPerfList;
class CInterval;
class CQImageButton;

class QCheckBox;
class QComboBox;
class QSpinBox;

class CQPerfDialog : public QDialog {
  Q_OBJECT

 public:
  static CQPerfDialog *instance();

  CQPerfDialog(QWidget *parent=nullptr);

 ~CQPerfDialog();

  void setTimeout(int t);

 private slots:
  void setName(const QString &name);
  void setNames(const QStringList &names);

  void updateWidgets();

  void enabledSlot(int state);
  void debugSlot(int state);
  void typeComboSlot(int ind);
  void shapeComboSlot(int ind);
  void valueComboSlot(int ind);
  void windowSizeSlot(int size);
  void recordSlot();
  void stateSlot();
  void zoomOutSlot();
  void zoomInSlot();
  void scrollSlot(int);

 private:
  QCheckBox*     enableCheck_    { nullptr };
  QCheckBox*     debugCheck_     { nullptr };
  QComboBox*     typeCombo_      { nullptr };
  QComboBox*     shapeCombo_     { nullptr };
  QComboBox*     valueCombo_     { nullptr };
  QSpinBox*      windowSizeSpin_ { nullptr };
  CQImageButton* recordButton_   { nullptr };
  CQPerfGraph*   graph_          { nullptr };
  QScrollBar*    graphScroll_    { nullptr };
  CQPerfList*    list_           { nullptr };
  int            timeout_        { 250 };
  QTimer*        timer_          { nullptr };
};

//---

class CQPerfGraph : public QFrame {
  Q_OBJECT

  Q_PROPERTY(bool showTotal     READ isShowTotal     WRITE setShowTotal    )
  Q_PROPERTY(bool showDepth     READ isShowDepth     WRITE setShowDepth    )
  Q_PROPERTY(bool showRecording READ isShowRecording WRITE setShowRecording)
  Q_PROPERTY(bool showPoints    READ isShowPoints    WRITE setShowPoints   )
  Q_PROPERTY(bool showRects     READ isShowRects     WRITE setShowRects    )
  Q_PROPERTY(bool showElapsed   READ isShowElapsed   WRITE setShowRects    )
  Q_PROPERTY(bool showCount     READ isShowCount     WRITE setShowCount    )
  Q_PROPERTY(int  windowSize    READ windowSize      WRITE setWindowSize   )
  Q_PROPERTY(int  numIntervals  READ numIntervals    WRITE setNumIntervals )

 public:
  CQPerfGraph(QWidget *parent=nullptr, const QString &name="");

 ~CQPerfGraph();

  void setName(const QString &name);
  void setNames(const QStringList &names);

  int windowSize() const { return windowSize_; }

  int numIntervals() const { return numIntervals_; }

  bool isShowTotal    () const { return showTotal_    ; }
  bool isShowDepth    () const { return showDepth_    ; }
  bool isShowRecording() const { return showRecording_; }

  bool isShowPoints() const { return showPoints_; }
  bool isShowRects () const { return showRects_; }

  bool isShowElapsed() const { return showElapsed_; }
  bool isShowCount  () const { return showCount_; }

  int zoomFactor() const { return zoomFactor_; }
  void setZoomFactor(int i) { zoomFactor_ = i; }

  double zoomOffset() const { return zoomOffset_; }
  void setZoomOffset(double r) { zoomOffset_ = r; }

  void contextMenuEvent(QContextMenuEvent *event);

  void paintEvent(QPaintEvent *);

  void drawIntervalGraph(QPainter *p);
  void drawDepthGraph   (QPainter *p);
  void drawStatsGraph   (QPainter *p);

  void drawAxis(QPainter *p, const CInterval &interval, bool isLeft, bool isCalls);

  void drawGrid(QPainter *p, const CInterval &interval);

  void zoomOut();
  void zoomIn();

  bool event(QEvent *e);

  QSize sizeHint() const { return QSize(600, 400); }

 public slots:
  void setWindowSize(int i) { windowSize_ = i; }

  void setNumIntervals(int i) { numIntervals_ = i; }

  void setShowTotal    (bool b) { showTotal_     = b; }
  void setShowDepth    (bool b) { showDepth_     = b; }
  void setShowRecording(bool b) { showRecording_ = b; }

  void setShowPoints(bool b) { showPoints_ = b; }
  void setShowRects (bool b) { showRects_  = b; }

  void setShowElapsed(bool b) { showElapsed_ = b; }
  void setShowCount  (bool b) { showCount_   = b; }

 private:
  void countToPixel  (double x, double y, double &px, double &py);
  void elapsedToPixel(double x, double y, double &px, double &py);

 private:
  struct TipRect {
    QRectF  rect;
    QString tip;

    TipRect(const QRectF &rect, const QString &tip) :
     rect(rect), tip(tip) {
    }
  };

  using TipRects = std::vector<TipRect>;

  QStringList names_;
  int         windowSize_    { 10000 };
  int         numIntervals_  { 50 };
  bool        showTotal_     { true };
  bool        showDepth_     { false };
  bool        showRecording_ { false };
  bool        showPoints_    { true };
  bool        showRects_     { false };
  bool        showElapsed_   { true };
  bool        showCount_     { true };
  int         zoomFactor_    { 1 };
  double      zoomOffset_    { 0.0 };
  double      xmin_          { 0.0 };
  double      xmax_          { 1.0 };
  double      ymin1_         { 0.0 };
  double      ymax1_         { 1.0 };
  double      ymin2_         { 0.0 };
  double      ymax2_         { 1.0 };
  int         lmargin_       { 0 };
  int         rmargin_       { 0 };
  int         tmargin_       { 0 };
  int         bmargin_       { 0 };
  QFont       axisFont_;
  TipRects    tipRects_;
};

//---

class CQPerfList : public QTableWidget {
  Q_OBJECT

  Q_PROPERTY(bool singleSelect READ isSingleSelect WRITE setSingleSelect)

 public:
  CQPerfList(QWidget *parent=nullptr);

  bool isSingleSelect() const { return singleSelect_; }
  void setSingleSelect(bool b) { singleSelect_ = b; }

  QSize sizeHint() const { return QSize(600, 400); }

 signals:
  void nameSelected(const QString &name);
  void namesSelected(const QStringList &names);

 public slots:
  void reload();

  void refresh();

 private slots:
  void selectionChangeSlot();

  void clickSlot(int row, int column);

 private:
  bool singleSelect_ { false };
};

#endif
