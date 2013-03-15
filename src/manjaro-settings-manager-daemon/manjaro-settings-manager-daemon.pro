#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T01:28:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = manjaro-settings-manager-daemon
TEMPLATE = app

QMAKE_CXXFLAGS += "-DDEFAULT_PATH_VALUE=global/"


SOURCES += main.cpp\
    daemon.cpp

HEADERS  += daemon.h

unix:!macx: LIBS += -L$$OUT_PWD/../global/ -lglobal

INCLUDEPATH += $$PWD/../global
DEPENDPATH += $$PWD/../global

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../global/libglobal.a

RESOURCES += \
    resources.qrc


target.path = /usr/bin
INSTALLS += target
