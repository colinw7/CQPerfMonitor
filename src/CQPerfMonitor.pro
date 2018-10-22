TEMPLATE = lib

TARGET = CQPerfMonitor

DEPENDPATH += .

QT += widgets

CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++14

MOC_DIR = .moc

SOURCES += \
CQPerfMonitor.cpp \
CQPerfGraph.cpp \

HEADERS += \
../include/CQPerfMonitor.h \
../include/CQPerfGraph.h \

OBJECTS_DIR = ../obj

DESTDIR = ../lib

INCLUDEPATH += \
. \
../include \
../../CQUtil/include \
../../CImageLib/include \
../../CFile/include \
../../CMath/include \
../../CStrUtil/include \
../../CUtil/include \
../../COS/include \
