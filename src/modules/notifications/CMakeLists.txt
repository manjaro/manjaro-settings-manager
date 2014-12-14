include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(MSM_NOTIFICATIONS_SRCS
    PageNotifications.cpp)

set(MSM_NOTIFICATIONS_UI
    ui/PageNotifications.ui)
ki18n_wrap_ui(MSM_NOTIFICATIONS_SRCS ${MSM_NOTIFICATIONS_UI})

add_library(msm_notifications MODULE ${MSM_NOTIFICATIONS_SRCS})
target_compile_definitions(msm_notifications PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_notifications
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::I18n)

install(TARGETS msm_notifications DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_notifications.desktop DESTINATION ${SERVICES_INSTALL_DIR})
