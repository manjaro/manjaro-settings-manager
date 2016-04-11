cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_TIMEDATE_GLOB
     msm_timedate.desktop)

set(MSM_TIMEDATE_SRCS
    ${TIMEDATE_MODULE_GLOB}
    LocaleGlobal.cpp
    TimeDate.cpp
    TimeDateModule.cpp
    TimeZoneDialog.cpp
    TimeZoneWidget.cpp)

set(MSM_TIMEDATE_UI
    ui/PageTimeDate.ui
    ui/TimeZoneDialog.ui)
qt5_wrap_ui(MSM_TIMEDATE_SRCS ${MSM_TIMEDATE_UI})

set(MSM_TIMEDATE_RESOURCES
    timedate.qrc)
QT5_ADD_RESOURCES (MSM_TIMEDATE_SRCS ${MSM_TIMEDATE_RESOURCES})

add_library(msm_timedate MODULE ${MSM_TIMEDATE_SRCS})
target_compile_definitions(msm_timedate PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_timedate
    msm
    Qt5::DBus
    Qt5::Widgets
    KF5::CoreAddons
    KF5::ConfigWidgets)

install(TARGETS msm_timedate DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_timedate.desktop DESTINATION ${SERVICES_INSTALL_DIR})

#msm_page_timedate library
set(MSM_TIMEDATE_PAGE_SRCS
    LocaleGlobal.cpp
    PageTimeDate.cpp
    TimeDate.cpp
    TimeZoneDialog.cpp
    TimeZoneWidget.cpp)
qt5_wrap_ui(MSM_TIMEDATE_PAGE_SRCS ${MSM_TIMEDATE_UI})
QT5_ADD_RESOURCES (MSM_TIMEDATE_PAGE_SRCS ${MSM_TIMEDATE_RESOURCES})

add_library(msm_page_timedate STATIC ${MSM_TIMEDATE_PAGE_SRCS})
target_compile_definitions(msm_page_timedate PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_timedate
    msm
    Qt5::DBus)
