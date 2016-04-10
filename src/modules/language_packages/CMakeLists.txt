include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_LANGUAGE_PACKAGES_GLOB
     msm_language_packages.desktop
     language_packages.json)

set(MSM_LANGUAGE_PACKAGES_SRCS
    ${MSM_LANGUAGE_PACKAGES_GLOB}
    LanguagePackagesModule.cpp)

set(MSM_LANGUAGE_PACKAGES_RESOURCES
    language_packages.qrc)
QT5_ADD_RESOURCES (MSM_LANGUAGE_PACKAGES_SRCS ${MSM_LANGUAGE_PACKAGES_RESOURCES})

set(MSM_LANGUAGE_PACKAGES_UI
    ui/PageLanguagePackages.ui)
qt5_wrap_ui(MSM_LANGUAGE_PACKAGES_SRCS ${MSM_LANGUAGE_PACKAGES_UI})

cmake_policy(SET CMP0063 NEW)

add_library(msm_language_packages MODULE ${MSM_LANGUAGE_PACKAGES_SRCS})
target_compile_definitions(msm_language_packages PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_language_packages
    msm
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::CoreAddons)

install(TARGETS msm_language_packages DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_language_packages.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kauth_install_actions(org.manjaro.msm.languagepackages msm_language_packages.actions)

add_executable(msm_language_packages_authhelper LanguagePackagesAuthHelper.cpp)
target_link_libraries(msm_language_packages_authhelper KF5::Auth KF5::ConfigCore)
install(TARGETS msm_language_packages_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_language_packages_authhelper org.manjaro.msm.languagepackages root)


#msm_page_language_packages library
set(MSM_LANGUAGE_PACKAGES_PAGE_SRCS
    PageLanguagePackages.cpp)
qt5_wrap_ui(MSM_LANGUAGE_PACKAGES_PAGE_SRCS ${MSM_LANGUAGE_PACKAGES_UI})
QT5_ADD_RESOURCES (MSM_LANGUAGE_PACKAGES_PAGE_SRCS ${MSM_LANGUAGE_PACKAGES_RESOURCES})

add_library(msm_page_language_packages STATIC ${MSM_LANGUAGE_PACKAGES_PAGE_SRCS})
target_compile_definitions(msm_page_language_packages PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_language_packages
    msm)
