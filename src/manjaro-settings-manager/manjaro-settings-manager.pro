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

SOURCES  += main.cpp \
            mainwindow.cpp \
            dialogs/accounttypedialog.cpp \
            dialogs/adduserdialog.cpp \
            dialogs/applydialog.cpp \
            dialogs/changepassworddialog.cpp \
            dialogs/previewfiledialog.cpp \
            pages/page_language.cpp \
            pages/page_languagepackages.cpp \
            pages/page_mhwd.cpp \
            pages/page_keyboard.cpp \
            pages/page_users.cpp \
            widgets/flatbutton.cpp \
            widgets/listwidget.cpp \
            widgets/keyboardpreview.cpp \
            widgets/pagewidget.cpp \
            widgets/selectlocalesdialog.cpp \
            widgets/toolbarstylewidget.cpp


HEADERS  += mainwindow.h \
            dialogs/accounttypedialog.h \
            dialogs/adduserdialog.h \
            dialogs/applydialog.h \
            dialogs/changepassworddialog.h \
            dialogs/previewfiledialog.h \
            pages/page_language.h \
            pages/page_languagepackages.h \
            pages/page_mhwd.h \
            pages/page_keyboard.h \
            pages/page_users.h \
            widgets/flatbutton.h \
            widgets/listwidget.h \
            widgets/keyboardpreview.h \
            widgets/pagewidget.h \
            widgets/selectlocalesdialog.h \
            widgets/toolbarstylewidget.h


FORMS    += mainwindow.ui \
            dialogs/adduserdialog.ui \
            dialogs/accounttypedialog.ui \
            dialogs/applydialog.ui \
            dialogs/changepassworddialog.ui \
            pages/page_keyboard.ui \
            pages/page_language.ui \
            pages/page_languagepackages.ui \
            pages/page_mhwd.ui \
            pages/page_users.ui \
            widgets/selectlocalesdialog.ui


TRANSLATIONS += translations/msm_ast.ts \
                translations/msm_ca.ts \
                translations/msm_cs_CZ.ts \
                translations/msm_de.ts \
                translations/msm_en.ts \
                translations/msm_el.ts \
                translations/msm_es.ts \
                translations/msm_fa.ts \
                translations/msm_hi.ts \
                translations/msm_it.ts \
                translations/msm_ja.ts \
                translations/msm_nb.ts \
                translations/msm_nl.ts \
                translations/msm_pl.ts \
                translations/msm_pt_BR.ts \
                translations/msm_ro.ts \
                translations/msm_sk_SK.ts \
                translations/msm_tr.ts \
                translations/msm_uk.ts

RESOURCES += \
    resources.qrc

unix:!macx: LIBS += -L$$OUT_PWD/../global/ -lglobal

INCLUDEPATH += $$PWD/../global
DEPENDPATH += $$PWD/../global

unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../global/libglobal.a



target.path = /usr/bin
INSTALLS += target
