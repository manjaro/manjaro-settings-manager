cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Common
file(GLOB TIMEDATE_GLOB
     images/background.png
     images/no.svg
     images/pin.png
     images/timezone_-1.0.png
     images/timezone_-10.0.png
     images/timezone_-11.0.png
     images/timezone_-2.0.png
     images/timezone_-3.0.png
     images/timezone_-3.5.png
     images/timezone_-4.0.png
     images/timezone_-4.5.png
     images/timezone_-5.0.png
     images/timezone_-5.5.png
     images/timezone_-6.0.png
     images/timezone_-7.0.png
     images/timezone_-8.0.png
     images/timezone_-9.0.png
     images/timezone_-9.5.png
     images/timezone_0.0.png
     images/timezone_1.0.png
     images/timezone_10.0.png
     images/timezone_10.5.png
     images/timezone_11.0.png
     images/timezone_11.5.png
     images/timezone_12.0.png
     images/timezone_12.75.png
     images/timezone_13.0.png
     images/timezone_2.0.png
     images/timezone_3.0.png
     images/timezone_3.5.png
     images/timezone_4.0.png
     images/timezone_4.5.png
     images/timezone_5.0.png
     images/timezone_5.5.png
     images/timezone_5.75.png
     images/timezone_6.0.png
     images/timezone_6.5.png
     images/timezone_7.0.png
     images/timezone_8.0.png
     images/timezone_9.0.png
     images/timezone_9.5.png
     images/yes.svg
     msm_timedate.desktop)

set(TIMEDATE_SRCS
    ${TIMEDATE_GLOB}
    LocaleGlobal.cpp    
    TimeDateCommon.cpp
    TimeZoneDialog.cpp
    TimeDateService.cpp
    TimeZoneWidget.cpp)

set(TIMEDATE_UI
    ui/PageTimeDate.ui
    ui/TimeZoneDialog.ui)
qt5_wrap_ui(TIMEDATE_SRCS ${TIMEDATE_UI})

set(TIMEDATE_RESOURCES
    timedate.qrc)
QT5_ADD_RESOURCES (TIMEDATE_SRCS ${TIMEDATE_RESOURCES})

# KCM Module
set(TIMEDATE_KCM_SRCS
    ${TIMEDATE_SRCS}
    TimeDateModule.cpp)
add_library(msm_timedate MODULE ${TIMEDATE_KCM_SRCS})
target_compile_definitions(msm_timedate PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_timedate
    msm
    Qt5::DBus
    Qt5::Widgets
    KF5::CoreAddons
    KF5::ConfigWidgets)

install(TARGETS msm_timedate DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_timedate.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# MSM Page
set(TIMEDATE_PAGE_SRCS
    ${TIMEDATE_SRCS}
    TimeDatePage.cpp)
add_library(msm_page_timedate STATIC ${TIMEDATE_PAGE_SRCS})
target_compile_definitions(msm_page_timedate PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_timedate
    msm
    Qt5::DBus)
