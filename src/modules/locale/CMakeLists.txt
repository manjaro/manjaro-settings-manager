include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_LOCALE_GLOB
     msm_locale.desktop)

set(MSM_LOCALE_SRCS
    ${MSM_LOCALE_GLOB}
    EnabledLocalesModel.cpp
    LanguageListViewDelegate.cpp
    LocaleModule.cpp
    SelectLocalesDialog.cpp
    SupportedLocalesItem.cpp
    SupportedLocalesModel.cpp)

set(MSM_LOCALE_UI
    ui/LocaleModule.ui
    ui/SelectLocalesDialog.ui)
qt5_wrap_ui(MSM_LOCALE_SRCS ${MSM_LOCALE_UI})

cmake_policy(SET CMP0063 NEW)

add_library(msm_locale MODULE ${MSM_LOCALE_SRCS})
target_compile_definitions(msm_locale PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_locale
    msm
    Qt5::DBus
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::ItemModels
    icudata
    icuuc)

install(TARGETS msm_locale DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES msm_locale.desktop DESTINATION ${SERVICES_INSTALL_DIR})

kauth_install_actions(org.manjaro.msm.locale msm_locale.actions)

add_executable(msm_locale_authhelper LocaleAuthHelper.cpp)
target_link_libraries(msm_locale_authhelper Qt5::DBus KF5::Auth KF5::ConfigCore)
install(TARGETS msm_locale_authhelper DESTINATION ${KAUTH_HELPER_INSTALL_DIR})
kauth_install_helper_files(msm_locale_authhelper org.manjaro.msm.locale root)
