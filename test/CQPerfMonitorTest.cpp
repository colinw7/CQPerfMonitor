#include <CQPerfMonitorTest.h>
#include <CQPerfMonitor.h>

#include <CQApp.h>
#include <QTimer>
#include <cmath>
#include <iostream>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  bool server = false;
  bool client = false;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      std::string arg = &argv[i][1];

      if      (arg == "server")
        server = true;
      else if (arg == "client")
        client = true;
      else
        std::cerr << "Invalid option '" << arg << "'\n";
    }
    else {
      std::cerr << "Invalid arg '" << argv[i] << "'\n";
    }
  }

  //---

  if      (server)
    CQPerfMonitorInst->createServer();
  else if (client)
    CQPerfMonitorInst->createClient();

  //---

  CQPerfMonitorTest test(server);

  test.show();

  app.exec();

  return 0;
}

//-----

CQPerfMonitorTest::
CQPerfMonitorTest(bool server)
{
  CQPerfTrace trace("CQPerfMonitorTest::CQPerfMonitorTest");

  if (! server) {
    timer_ = new QTimer(this);

    connect(timer_, SIGNAL(timeout()), this, SLOT(timerSlot()));

    timer_->start(1000);
  }
}

CQPerfMonitorTest::
~CQPerfMonitorTest()
{
}

void
CQPerfMonitorTest::
timerSlot()
{
  CQPerfTrace trace("CQPerfMonitorTest::timerSlot");

  double x { 0.0 };

  for (int i = 0; i < 10000; ++i)
    x += cos(i)*cos(i) + sin(i)*sin(i);

  assert(x != 1E50);
}
