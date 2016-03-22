include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_GLOB
     msm.desktop)

set(MSM_SRCS
    ${MSM_GLOB}
    main.cpp
    MsmApplication.cpp
    MsmWindow.cpp
    delegates/LanguageListViewDelegate.cpp
    dialogs/AccountTypeDialog.cpp
    dialogs/AddUserDialog.cpp
    dialogs/ChangePasswordDialog.cpp
    dialogs/SelectLocalesDialog.cpp
    dialogs/TimeZoneDialog.cpp
    dialogs/PreviewFileDialog.cpp
    global/Global.cpp
    global/Const.h
    mhwd/mhwd.cpp
    mhwd/vita/string.cpp
    models/EnabledLocalesModel.cpp
    models/KeyboardModel.cpp
    models/KeyboardItem.cpp
    models/SupportedLocalesItem.cpp
    models/SupportedLocalesModel.cpp
    models/TimeDate.cpp
    pages/PageKeyboard.cpp
    pages/PageLanguage.cpp
    pages/PageLanguagePackages.cpp
    pages/PageMhwd.cpp
    pages/PageNotifications.cpp
    pages/PageTimeDate.cpp
    pages/PageUsers.cpp
    pages/SetKeyboardLayoutJob.cpp
    widgets/FlatButton.cpp
    widgets/KeyboardPreview.cpp
    widgets/ListWidget.cpp
    widgets/LocaleGlobal.cpp
    widgets/TimeZoneWidget.cpp)

set(MSM_UI
    MsmWindow.ui
    dialogs/AccountTypeDialog.ui
    dialogs/AddUserDialog.ui
    dialogs/ChangePasswordDialog.ui
    dialogs/SelectLocalesDialog.ui
    dialogs/TimeZoneDialog.ui
    pages/PageKeyboard.ui
    pages/PageLanguage.ui
    pages/PageLanguagePackages.ui
    pages/PageMhwd.ui
    pages/PageNotifications.ui
    pages/PageTimeDate.ui
    pages/PageUsers.ui)
qt5_wrap_ui(MSM_SRCS ${MSM_UI})

set(MSM_RESOURCES
    msm.qrc)
QT5_ADD_RESOURCES (MSM_SRCS ${MSM_RESOURCES})

add_executable(msm_bin ${MSM_SRCS})
SET_TARGET_PROPERTIES(msm_bin
    PROPERTIES
        AUTOMOC TRUE
        ENABLE_EXPORTS TRUE
        RUNTIME_OUTPUT_NAME msm)
target_link_libraries(msm_bin
    msm_page_kernel
    Qt5::Core
    Qt5::Widgets
    Qt5::DBus
    KF5::ConfigWidgets
    KF5::KCMUtils
    KF5::IconThemes
    KF5::ItemModels
    hd
    icudata
    icuuc)

install( TARGETS msm_bin
    # BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
# install(FILES msm.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
