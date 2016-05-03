cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Common
file(GLOB USERS_GLOB
     icons/add.png
     icons/adduser.png
     icons/lock.png
     icons/remove.png
     icons/user.png
     icons/users.png
     msm_users.desktop)

set(USERS_SRCS
    ${USERS_GLOB}
    AccountTypeDialog.cpp
    AddUserDialog.cpp
    ChangePasswordDialog.cpp
    FlatButton.cpp
    UsersCommon.cpp
    PreviewFileDialog.cpp)

set(USERS_UI
    ui/AccountTypeDialog.ui
    ui/AddUserDialog.ui
    ui/ChangePasswordDialog.ui
    ui/PageUsers.ui)
qt5_wrap_ui(USERS_SRCS ${USERS_UI})

set(USERS_RESOURCES
    users.qrc)
QT5_ADD_RESOURCES (USERS_SRCS ${USERS_RESOURCES})

# KCM Module
set(USERS_KCM_SRCS
    ${USERS_SRCS}
    UsersModule.cpp)

add_library(msm_users MODULE ${USERS_KCM_SRCS})
target_compile_definitions(msm_users PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_users
    msm
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::CoreAddons)

install(TARGETS msm_users DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_users.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# MSM Page
set(USERS_PAGE_SRCS
    ${USERS_SRCS}
    UsersPage.cpp)

add_library(msm_page_users STATIC ${USERS_PAGE_SRCS})
target_compile_definitions(msm_page_users PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_users
    msm)

# Auth Helper
add_executable(msm_users_authhelper UsersAuthHelper.cpp)
target_link_libraries(msm_users_authhelper
    KF5::Auth)

install(TARGETS msm_users_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_users_authhelper org.manjaro.msm.users root)
kauth_install_actions(org.manjaro.msm.users msm_users.actions)
