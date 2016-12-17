cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Common
file(GLOB LANGUAGE_PACKAGES_GLOB
     msm_language_packages.desktop)

set(LANGUAGE_PACKAGES_SRCS
    ${LANGUAGE_PACKAGES_GLOB}
    LanguagePackagesCommon.cpp)

set(LANGUAGE_PACKAGES_UI
    ui/PageLanguagePackages.ui)
qt5_wrap_ui(LANGUAGE_PACKAGES_SRCS ${LANGUAGE_PACKAGES_UI})

# KCM Module
set(LANGUAGE_PACKAGES_KCM_SRCS
    ${LANGUAGE_PACKAGES_SRCS}
    LanguagePackagesModule.cpp)

add_library(msm_language_packages MODULE ${LANGUAGE_PACKAGES_KCM_SRCS})
target_compile_definitions(msm_language_packages PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_language_packages
    msm
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::CoreAddons)

install(TARGETS msm_language_packages DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_language_packages.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# MSM Page
set(LANGUAGE_PACKAGES_PAGE_SRCS
    ${LANGUAGE_PACKAGES_SRCS}
    LanguagePackagesPage.cpp)

add_library(msm_page_language_packages STATIC ${LANGUAGE_PACKAGES_PAGE_SRCS})
target_compile_definitions(msm_page_language_packages PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_language_packages
    msm)

# Auth Helper
add_executable(msm_language_packages_authhelper LanguagePackagesAuthHelper.cpp)
target_link_libraries(msm_language_packages_authhelper
    KF5::Auth)

install(TARGETS msm_language_packages_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_language_packages_authhelper org.manjaro.msm.languagepackages root)
kauth_install_actions(org.manjaro.msm.languagepackages msm_language_packages.actions)
