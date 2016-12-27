cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Common
file(GLOB KEYBOARD_GLOB
     icons/restore.png
     kbd-model-map
     msm_keyboard.desktop)

set(KEYBOARD_SRCS
    ${KEYBOARD_GLOB}
    KeyboardCommon.cpp
    KeyboardItem.cpp
    KeyboardModel.cpp    
    KeyboardPreview.cpp)

set(KEYBOARD_UI
    ui/PageKeyboard.ui)
qt5_wrap_ui(KEYBOARD_SRCS ${KEYBOARD_UI})

set(KEYBOARD_RESOURCES
    keyboard.qrc)
QT5_ADD_RESOURCES (KEYBOARD_SRCS ${KEYBOARD_RESOURCES})

# KCM Module
set(KEYBOARD_KCM_SRCS
    ${KEYBOARD_SRCS}
    KeyboardModule.cpp)
add_library(msm_keyboard MODULE ${KEYBOARD_KCM_SRCS})
target_compile_definitions(msm_keyboard PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_keyboard
    msm
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::ItemModels)

install(TARGETS msm_keyboard DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_keyboard.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# MSM Page
set(KEYBOARD_PAGE_SRCS
    ${KEYBOARD_SRCS}
    KeyboardPage.cpp
    SetKeyboardLayoutJob.cpp)
add_library(msm_page_keyboard STATIC ${KEYBOARD_PAGE_SRCS})
target_compile_definitions(msm_page_keyboard PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_keyboard
    msm
    Qt5::DBus
    KF5::ItemModels)

# Auth Helper
add_executable(msm_keyboard_authhelper
    KeyboardAuthHelper.cpp
    SetKeyboardLayoutJob.cpp)
target_link_libraries(msm_keyboard_authhelper
    Qt5::DBus
    KF5::Auth)

install(TARGETS msm_keyboard_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_keyboard_authhelper org.manjaro.msm.keyboard root)
kauth_install_actions(org.manjaro.msm.keyboard msm_keyboard.actions)
