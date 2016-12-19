cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Common
file(GLOB LOCALE_GLOB
     msm_locale.desktop)

set(LOCALE_SRCS
    ${LOCALE_GLOB}
    EnabledLocalesModel.cpp
    LanguageListViewDelegate.cpp
    LocaleCommon.cpp
    SelectLocalesDialog.cpp
    SupportedLocalesItem.cpp
    SupportedLocalesModel.cpp)

set(LOCALE_UI
    ui/LocaleModule.ui
    ui/SelectLocalesDialog.ui)
qt5_wrap_ui(LOCALE_SRCS ${LOCALE_UI})

# KCM Module
set(LOCALE_KCM_SRCS
    ${LOCALE_SRCS}
    LocaleModule.cpp)

add_library(msm_locale MODULE ${LOCALE_KCM_SRCS})
target_compile_definitions(msm_locale PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_locale    
    Qt5::DBus
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::ItemModels
    icudata
    icuuc
    msm)

install(TARGETS msm_locale DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_locale.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# MSM PAGE
set(LOCALE_PAGE_SRCS
    ${LOCALE_SRCS}
    LocalePage.cpp)

add_library(msm_page_locale STATIC ${LOCALE_PAGE_SRCS})
target_compile_definitions(msm_page_locale PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_page_locale
    msm
    Qt5::DBus
    KF5::ItemModels
    icudata
    icuuc)

# Auth Helper
add_executable(msm_locale_authhelper LocaleAuthHelper.cpp)
target_link_libraries(msm_locale_authhelper
    msm
    Qt5::DBus
    KF5::Auth)

install(TARGETS msm_locale_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_locale_authhelper org.manjaro.msm.locale root)
kauth_install_actions(org.manjaro.msm.locale msm_locale.actions)
