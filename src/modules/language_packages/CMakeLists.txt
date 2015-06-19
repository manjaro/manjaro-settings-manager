include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_LANGUAGE_PACKAGES_GLOB
     msm_language_packages.desktop)

set(MSM_LANGUAGE_PACKAGES_SRCS
    ${MSM_LANGUAGE_PACKAGES_GLOB}
    global.cpp
    PageLanguagePackages.cpp)

set(MSM_LANGUAGE_PACKAGES_UI
    ui/PageLanguagePackages.ui)
ki18n_wrap_ui(MSM_LANGUAGE_PACKAGES_SRCS ${MSM_LANGUAGE_PACKAGES_UI})

add_library(msm_language_packages MODULE ${MSM_LANGUAGE_PACKAGES_SRCS})
target_compile_definitions(msm_language_packages PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_language_packages
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::I18n)

install(TARGETS msm_language_packages DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_language_packages.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kauth_install_actions(org.manjaro.msm.languagepackages msm_language_packages.actions)

add_executable(msm_language_packages_authhelper LanguagePackagesAuthHelper.cpp)
target_link_libraries(msm_language_packages_authhelper KF5::Auth KF5::ConfigCore)
install(TARGETS msm_language_packages_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_language_packages_authhelper org.manjaro.msm.languagepackages root)
