include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_KERNEL_GLOB
     msm_kernel.desktop
     changelogs/linux34.html
     changelogs/linux38.html
     changelogs/linux40.html
     changelogs/linux310.html
     changelogs/linux311.html
     changelogs/linux312.html
     changelogs/linux313.html
     changelogs/linux314.html
     changelogs/linux315.html
     changelogs/linux316.html
     changelogs/linux317.html
     changelogs/linux318.html
     changelogs/linux319.html)

set(MSM_KERNEL_SRCS
    ${MSM_KERNEL_GLOB}
    KernelInfoDialog.cpp
    KernelListViewDelegate.cpp
    KernelModule.cpp)

set(MSM_KERNEL_RESOURCES
    kernel.qrc)
QT5_ADD_RESOURCES (MSM_KERNEL_SRCS ${MSM_KERNEL_RESOURCES})

add_library(msm_kernel MODULE ${MSM_KERNEL_SRCS})
target_compile_definitions(msm_kernel PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_kernel
    msm
    KF5::ConfigWidgets
    KF5::CoreAddons)


install(TARGETS msm_kernel DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_kernel.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kauth_install_actions(org.manjaro.msm.kernel msm_kernel.actions)

add_executable(msm_kernel_authhelper KernelAuthHelper.cpp)
target_link_libraries(msm_kernel_authhelper KF5::Auth KF5::ConfigCore)
install(TARGETS msm_kernel_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_kernel_authhelper org.manjaro.msm.kernel root)
