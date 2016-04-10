include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_USERS_GLOB
     msm_users.desktop)

set(MSM_USERS_SRCS
    ${MSM_USERS_GLOB}
    AccountTypeDialog.cpp
    AddUserDialog.cpp
    ChangePasswordDialog.cpp
    FlatButton.cpp
    global.cpp
    ListWidget.cpp
    PreviewFileDialog.cpp
    UsersModule.cpp)

set(MSM_USERS_UI
    ui/AccountTypeDialog.ui
    ui/AddUserDialog.ui
    ui/ChangePasswordDialog.ui
    ui/PageUsers.ui)
qt5_wrap_ui(MSM_USERS_SRCS ${MSM_USERS_UI})

set(MSM_USERS_RESOURCES
    users.qrc)
QT5_ADD_RESOURCES (MSM_USERS_SRCS ${MSM_USERS_RESOURCES})

cmake_policy(SET CMP0063 NEW)

add_library(msm_users MODULE ${MSM_USERS_SRCS})
target_compile_definitions(msm_users PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_users
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::CoreAddons)

install(TARGETS msm_users DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_users.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kauth_install_actions(org.manjaro.msm.users msm_users.actions)

add_executable(msm_users_authhelper UsersAuthHelper.cpp)
target_link_libraries(msm_users_authhelper KF5::Auth KF5::ConfigCore)
install(TARGETS msm_users_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_users_authhelper org.manjaro.msm.users root)


#msm_users_page library
set(MSM_USERS_PAGE_SRCS
    AccountTypeDialog.cpp
    AddUserDialog.cpp
    ChangePasswordDialog.cpp
    FlatButton.cpp
    global.cpp
    ListWidget.cpp
    PageUsers.cpp
    PreviewFileDialog.cpp)
qt5_wrap_ui(MSM_USERS_PAGE_SRCS ${MSM_USERS_UI})
QT5_ADD_RESOURCES (MSM_USERS_PAGE_SRCS ${MSM_USERS_RESOURCES})

add_library(msm_page_users STATIC ${MSM_USERS_PAGE_SRCS})
target_compile_definitions(msm_page_users PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_users
    msm)
