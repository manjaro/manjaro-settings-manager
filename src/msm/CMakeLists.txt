cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_GLOB
     manjaro-settings-manager.desktop)

set(MSM_SRCS
    ${MSM_GLOB}
    main.cpp
    MsmApplication.cpp
    MsmWindow.cpp)

set(MSM_UI
    MsmWindow.ui)
qt5_wrap_ui(MSM_SRCS ${MSM_UI})

set(MSM_RESOURCES
    msm.qrc)
QT5_ADD_RESOURCES (MSM_SRCS ${MSM_RESOURCES})

add_executable(msm_bin ${MSM_SRCS})
SET_TARGET_PROPERTIES(msm_bin
    PROPERTIES
    AUTOMOC TRUE
    ENABLE_EXPORTS TRUE
    RUNTIME_OUTPUT_NAME manjaro-settings-manager)
target_link_libraries(msm_bin
    msm_page_kernel
    msm_page_keyboard
    msm_page_language_packages
    msm_page_locale
    msm_page_mhwd
    msm_page_timedate
    msm_page_users
    msm
    Qt5::Core
    Qt5::Widgets)

install(TARGETS msm_bin
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES
    manjaro-settings-manager.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
