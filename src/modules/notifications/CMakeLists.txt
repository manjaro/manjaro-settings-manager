cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(MSM_NOTIFICATIONS_PAGE_SRCS
    PageNotifications.cpp)
set(MSM_NOTIFICATIONS_UI
    ui/PageNotifications.ui)
qt5_wrap_ui(MSM_NOTIFICATIONS_PAGE_SRCS ${MSM_NOTIFICATIONS_UI})
QT5_ADD_RESOURCES (MSM_NOTIFICATIONS_PAGE_SRCS ${MSM_TIMEDATE_RESOURCES})

add_library(msm_page_notifications STATIC ${MSM_NOTIFICATIONS_PAGE_SRCS})
target_compile_definitions(msm_page_notifications PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_notifications
    msm)
