cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB TIMEDATE_MODULE_GLOB
     msm_timedate.desktop)

set(TIMEDATE_MODULE_SRCS
    ${TIMEDATE_MODULE_GLOB}
    LocaleGlobal.cpp
    TimeDate.cpp
    TimeDateModule.cpp
    TimeZoneDialog.cpp
    TimeZoneWidget.cpp)

set(TIMEDATE_MODULE_UI
    ui/TimeDateModule.ui
    ui/TimeZoneDialog.ui)
qt5_wrap_ui(TIMEDATE_MODULE_SRCS ${TIMEDATE_MODULE_UI})

set(TIMEDATE_MODULE_RESOURCES
    timedate.qrc)
QT5_ADD_RESOURCES (TIMEDATE_MODULE_SRCS ${TIMEDATE_MODULE_RESOURCES})

add_library(timedate_module STATIC ${TIMEDATE_MODULE_SRCS})
target_compile_definitions(timedate_module PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(timedate_module
    Qt5::DBus
    Qt5::Widgets
    KF5::CoreAddons)

# KCM MODULE
set(MSM_TIMEDATE_SRCS
    TimeDateKcm.cpp)
QT5_ADD_RESOURCES (MSM_TIMEDATE_SRCS ${TIMEDATE_MODULE_RESOURCES})

add_library(msm_timedate MODULE  ${MSM_TIMEDATE_SRCS})
target_compile_definitions(msm_timedate PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_timedate
    timedate_module
    KF5::ConfigWidgets)

install(TARGETS msm_timedate DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_timedate.desktop DESTINATION ${SERVICES_INSTALL_DIR})
