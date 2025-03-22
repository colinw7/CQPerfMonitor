#ifndef CQDataModelTest_H
#define CQDataModelTest_H

#include <QFrame>

class QTimer;

class CQPerfMonitorTest : public QFrame {
  Q_OBJECT

 public:
  CQPerfMonitorTest(bool server);
 ~CQPerfMonitorTest();

  QSize sizeHint() const override { return QSize(800, 600); }

 private slots:
  void timerSlot();

 private:
  QTimer *timer_ { nullptr };
};

#endif
