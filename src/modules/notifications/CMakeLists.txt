cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})


set(NOTIFICATIONS_UI
    ui/PageNotifications.ui)
qt5_wrap_ui(NOTIFICATIONS_PAGE_SRCS ${NOTIFICATIONS_UI})


# KCM Module
set(NOTIFICATIONS_KCM_SRCS
    NotificationsModule.cpp)
add_library(msm_notifications MODULE ${NOTIFICATIONS_KCM_SRCS})
target_compile_definitions(msm_notifications PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_notifications
    msm
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::CoreAddons)

install(TARGETS msm_notifications DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_notifications.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# MSM PAGE
set(NOTIFICATIONS_PAGE_SRCS
    PageNotifications.cpp)

add_library(msm_page_notifications STATIC ${NOTIFICATIONS_PAGE_SRCS})
target_compile_definitions(msm_page_notifications PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_notifications
    msm)
