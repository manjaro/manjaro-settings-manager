#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T01:28:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    dialogs/previewfiledialog.cpp

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
    dialogs/previewfiledialog.h

FORMS    += mainwindow.ui \
    pages/page_languagepackages.ui \
    dialogs/applydialog.ui \
    pages/page_keyboard.ui \
    pages/page_language.ui \
    widgets/selectlocalesdialog.ui \
    pages/page_users.ui \
    dialogs/adduserdialog.ui \
    dialogs/changepassworddialog.ui \
    dialogs/accounttypedialog.ui

TRANSLATIONS += translations/msm_en.ts \
                translations/msm_ca.ts \
                translations/msm_de.ts

RESOURCES += \
    resources.qrc

unix:!macx: LIBS += -L$$OUT_PWD/../global/ -lglobal

INCLUDEPATH += $$PWD/../global
DEPENDPATH += $$PWD/../global

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../global/libglobal.a



target.path = /usr/bin
INSTALLS += target
