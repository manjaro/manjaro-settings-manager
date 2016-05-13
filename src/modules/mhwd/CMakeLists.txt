cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Common
file(GLOB MHWD_GLOB
     icons/add.png
     icons/ati.png
     icons/gpudriver.png
     icons/intel-nvidia.png
     icons/intel.png
     icons/nvidia.png
     icons/remove.png
     icons/restore.png
     msm_mhwd.desktop)

set(MHWD_SRCS
    ${MHWD_GLOB}
    libmhwd/mhwd.cpp
    libmhwd/vita/string.cpp
    MhwdCommon.cpp)

set(MHWD_UI
    ui/PageMhwd.ui)
qt5_wrap_ui(MHWD_SRCS ${MHWD_UI})

set(MHWD_RESOURCES
    mhwd.qrc)
QT5_ADD_RESOURCES (MHWD_SRCS ${MHWD_RESOURCES})

# KCM Module
set(MHWD_KCM_SRCS
    ${MHWD_SRCS}
    MhwdModule.cpp)
add_library(msm_mhwd MODULE ${MHWD_KCM_SRCS})
target_compile_definitions(msm_mhwd PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_mhwd
    msm
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::CoreAddons
    hd)

install(TARGETS msm_mhwd DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_mhwd.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# MSM PAGE
set(MHWD_PAGE_SRCS
    ${MHWD_SRCS}
    MhwdPage.cpp)
add_library(msm_page_mhwd STATIC ${MHWD_PAGE_SRCS})
target_compile_definitions(msm_page_mhwd PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_mhwd
    msm
    Qt5::Widgets
    hd)

# Auth Helper
add_executable(msm_mhwd_authhelper MhwdAuthHelper.cpp)
target_link_libraries(msm_mhwd_authhelper
    KF5::Auth)

install(TARGETS msm_mhwd_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_mhwd_authhelper org.manjaro.msm.mhwd root)
kauth_install_actions(org.manjaro.msm.mhwd msm_mhwd.actions)
