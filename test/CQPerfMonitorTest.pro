TEMPLATE = app

TARGET = CQPerfMonitorTest

QT += widgets svg

DEPENDPATH += .

QMAKE_CXXFLAGS += \
-std=c++14 \

MOC_DIR = .moc

SOURCES += \
CQPerfMonitorTest.cpp \

HEADERS += \
CQPerfMonitorTest.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj

INCLUDEPATH += \
. \
../include \
../../CQUtil/include \
../../CUtil/include \
../../COS/include \

unix:LIBS += \
-L../lib \
-L../../CQUtil/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CConfig/lib \
-L../../CFileUtil/lib \
-L../../CFile/lib \
-L../../CMath/lib \
-L../../CUtil/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../COS/lib \
-lCQPerfMonitor \
-lCQUtil \
-lCImageLib \
-lCFont \
-lCConfig \
-lCFileUtil \
-lCFile \
-lCMath \
-lCUtil \
-lCStrUtil \
-lCRegExp \
-lCOS \
-lpng -ljpeg -ltre
