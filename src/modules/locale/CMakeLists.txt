include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(MSM_LOCALE_SRCS
    EnabledLocalesModel.cpp
    LanguageListViewDelegate.cpp
    LocaleModule.cpp
    SelectLocalesDialog.cpp
    SupportedLocalesItem.cpp
    SupportedLocalesModel.cpp)

set(MSM_LOCALE_UI
    ui/LocaleModule.ui
    ui/SelectLocalesDialog.ui)
ki18n_wrap_ui(MSM_LOCALE_SRCS ${MSM_LOCALE_UI})

add_library(msm_locale MODULE ${MSM_LOCALE_SRCS})
target_compile_definitions(msm_locale PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_locale
    Qt5::DBus
    KF5::ConfigWidgets
    KF5::CoreAddons
    KF5::I18n
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
