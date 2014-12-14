project(manjaro-settings-manager)
SET(PROJECT_VERSION "1.0.0")

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(QT_MIN_VERSION "5.3.0")
find_package(ECM 1.4.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${PROJECT_SOURCE_DIR})

include(ECMInstallIcons)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets WebKitWidgets DBus)
find_package(KF5 REQUIRED COMPONENTS
    CoreAddons
    I18n
    Auth
    ConfigWidgets
    ItemModels
)
find_package(ICU REQUIRED)

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
