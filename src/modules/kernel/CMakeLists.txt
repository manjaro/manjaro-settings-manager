include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_KERNEL_GLOB
     msm_kernel.desktop)

set(MSM_KERNEL_SRCS
    ${MSM_KERNEL_GLOB}
    Kernel.cpp
    KernelModel.cpp
    KernelListViewDelegate.cpp
    KernelInfoDialog.cpp
    PageKernel.cpp)

set(MSM_KERNEL_UI
    ui/PageKernel.ui
    ui/KernelInfoDialog.ui)
ki18n_wrap_ui(MSM_KERNEL_SRCS ${MSM_KERNEL_UI})

set(MSM_KERNEL_RESOURCES
    kernel.qrc)
QT5_ADD_RESOURCES (MSM_KERNEL_SRCS ${MSM_KERNEL_RESOURCES})

add_library(msm_kernel MODULE ${MSM_KERNEL_SRCS})
target_compile_definitions(msm_kernel PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_kernel
    Qt5::WebKitWidgets
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::I18n)

install(TARGETS msm_kernel DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_kernel.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kauth_install_actions(org.manjaro.msm.kernel msm_kernel.actions)

add_executable(msm_kernel_authhelper KernelAuthHelper.cpp)
target_link_libraries(msm_kernel_authhelper KF5::Auth KF5::ConfigCore)
install(TARGETS msm_kernel_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_kernel_authhelper org.manjaro.msm.kernel root)
