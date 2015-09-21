include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_KEYBOARD_GLOB
     msm_keyboard.desktop)

set(MSM_KEYBOARD_SRCS
    ${MSM_KEYBOARD_GLOB}
    KeyboardItem.cpp
    KeyboardModel.cpp
    KeyboardModule.cpp
    KeyboardPreview.cpp)

set(MSM_KEYBOARD_UI
    ui/PageKeyboard.ui)
qt5_wrap_ui(MSM_KEYBOARD_SRCS ${MSM_KEYBOARD_UI})

set(MSM_KEYBOARD_RESOURCES
    keyboard.qrc)
QT5_ADD_RESOURCES (MSM_KEYBOARD_SRCS ${MSM_KEYBOARD_RESOURCES})

add_library(msm_keyboard MODULE ${MSM_KEYBOARD_SRCS})
target_compile_definitions(msm_keyboard PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_keyboard
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::ItemModels)

install(TARGETS msm_keyboard DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_keyboard.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kauth_install_actions(org.manjaro.msm.keyboard msm_keyboard.actions)

add_executable(msm_keyboard_authhelper KeyboardAuthHelper.cpp SetKeyboardLayoutJob.cpp)
target_link_libraries(msm_keyboard_authhelper KF5::Auth KF5::ConfigCore Qt5::DBus)
install(TARGETS msm_keyboard_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_keyboard_authhelper org.manjaro.msm.keyboard root)
