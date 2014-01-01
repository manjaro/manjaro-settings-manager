#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T01:28:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = manjaro-settings-manager-daemon
TEMPLATE = app

SOURCES += main.cpp\
    daemon.cpp

HEADERS  += daemon.h

unix:!macx: LIBS += -L$$OUT_PWD/../global/ -lglobal

INCLUDEPATH += $$PWD/../global
DEPENDPATH += $$PWD/../global

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../global/libglobal.a

TRANSLATIONS += translations/msmd_ca.ts \
                translations/msmd_cs-CZ.ts \
                translations/msmd_de.ts \
                translations/msmd_en.ts \
                translations/msmd_el.ts \
                translations/msmd_es.ts \
                translations/msmd_fa.ts \
                translations/msmd_hi.ts \
                translations/msmd_it.ts \
                translations/msmd_ja.ts \
                translations/msmd_nl.ts \
                translations/msmd_pl.ts \
                translations/msmd_pt-BR.ts \
                translations/msmd_ro.ts \
                translations/msmd_tr.ts

RESOURCES += \
    resources.qrc


target.path = /usr/bin
INSTALLS += target



include(qtsingleapplication/src/qtsingleapplication.pri)
