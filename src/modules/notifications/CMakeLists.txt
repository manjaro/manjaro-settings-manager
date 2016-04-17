cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(NOTIFICATIONS_PAGE_SRCS
    PageNotifications.cpp)
set(NOTIFICATIONS_UI
    ui/PageNotifications.ui)
qt5_wrap_ui(NOTIFICATIONS_PAGE_SRCS ${NOTIFICATIONS_UI})

add_library(msm_page_notifications STATIC ${NOTIFICATIONS_PAGE_SRCS})
target_compile_definitions(msm_page_notifications PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_notifications
    msm)
