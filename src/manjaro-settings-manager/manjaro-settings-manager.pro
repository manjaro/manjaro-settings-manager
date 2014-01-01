#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T01:28:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lmhwd
TARGET = manjaro-settings-manager-gui
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    widgets/toolbarstylewidget.cpp \
    widgets/listwidget.cpp \
    pages/page_languagepackages.cpp \
    widgets/pagewidget.cpp \
    dialogs/applydialog.cpp \
    pages/page_keyboard.cpp \
    widgets/keyboardpreview.cpp \
    pages/page_language.cpp \
    widgets/selectlocalesdialog.cpp \
    pages/page_users.cpp \
    widgets/flatbutton.cpp \
    dialogs/adduserdialog.cpp \
    dialogs/changepassworddialog.cpp \
    dialogs/accounttypedialog.cpp \
    dialogs/previewfiledialog.cpp \
    pages/page_gpudriver.cpp

HEADERS  += mainwindow.h \
    widgets/toolbarstylewidget.h \
    widgets/listwidget.h \
    pages/page_languagepackages.h \
    widgets/pagewidget.h \
    dialogs/applydialog.h \
    pages/page_keyboard.h \
    widgets/keyboardpreview.h \
    pages/page_language.h \
    widgets/selectlocalesdialog.h \
    pages/page_users.h \
    widgets/flatbutton.h \
    dialogs/adduserdialog.h \
    dialogs/changepassworddialog.h \
    dialogs/accounttypedialog.h \
    dialogs/previewfiledialog.h \
    pages/page_gpudriver.h

FORMS    += mainwindow.ui \
    pages/page_languagepackages.ui \
    dialogs/applydialog.ui \
    pages/page_keyboard.ui \
    pages/page_language.ui \
    widgets/selectlocalesdialog.ui \
    pages/page_users.ui \
    dialogs/adduserdialog.ui \
    dialogs/changepassworddialog.ui \
    dialogs/accounttypedialog.ui \
    pages/page_gpudriver.ui

TRANSLATIONS += translations/msm_ca.ts \
                translations/msm_cs-CZ.ts \
                translations/msm_de.ts \
                translations/msm_en.ts \
                translations/msm_el.ts \
                translations/msm_es.ts \
                translations/msm_fa.ts \
                translations/msm_hi.ts \
                translations/msm_it.ts \
                translations/msm_ja.ts \
                translations/msm_nl.ts \
                translations/msm_pl.ts \
                translations/msm_pt-BR.ts \
                translations/msm_ro.ts \
                translations/msm_tr.ts

RESOURCES += \
    resources.qrc

unix:!macx: LIBS += -L$$OUT_PWD/../global/ -lglobal

INCLUDEPATH += $$PWD/../global
DEPENDPATH += $$PWD/../global

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../global/libglobal.a



target.path = /usr/bin
INSTALLS += target
