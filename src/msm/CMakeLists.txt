include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_GLOB
     msm.desktop)

set(MSM_SRCS
    ${MSM_GLOB}
    main.cpp
    MsmApplication.cpp
    MsmWindow.cpp
    pages/PageNotifications.cpp)

set(MSM_UI
    MsmWindow.ui
    pages/PageNotifications.ui)
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
    msm_page_keyboard
    msm_page_language_packages
    msm_page_locale
    msm_page_mhwd
    msm_page_timedate
    msm_page_users
    msm
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
