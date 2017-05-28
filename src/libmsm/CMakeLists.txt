cmake_policy(SET CMP0063 NEW)

file(GLOB LIBMSM_GLOB
     language_packages.json)

file(GLOB LIBMSM_TS_GLOB
     translations/*.ts)

set(KDSINGLEAPPLICATIONGUARD
    kdsingleapplicationguard/kdsingleapplicationguard.cpp
    kdsingleapplicationguard/kdsharedmemorylocker.cpp
    kdsingleapplicationguard/kdtoolsglobal.cpp
    kdsingleapplicationguard/kdlockedsharedmemorypointer.cpp)

set_source_files_properties(
    ${KDSINGLEAPPLICATIONGUARD}
    PROPERTIES
    COMPILE_FLAGS "-Wno-unused-function")

set(LIBMSM_SRCS
    ${LIBMSM_GLOB}
    ${KDSINGLEAPPLICATIONGUARD}
    ActionDialog.cpp
    Kernel.cpp
    KernelModel.cpp
    LanguageCommon.cpp
    LanguagePackages.cpp
    LanguagePackagesItem.cpp
    ListWidget.cpp
    MsmCommon.cpp
    NotifierSettingsDialog.cpp
    PacmanUtils.cpp
    PageWidget.cpp)

set(LIBMSM_UI
    NotifierSettingsDialog.ui)
qt5_wrap_ui(LIBMSM_SRCS ${LIBMSM_UI})

set(translations_qrc "translations.qrc")
configure_file(${translations_qrc} ${translations_qrc} COPYONLY)
qt5_add_translation(LIBMSM_QM ${LIBMSM_TS_GLOB})

set(LIBMSM_RESOURCES
    language_packages.qrc
    ${CMAKE_BINARY_DIR}/src/libmsm/${translations_qrc})
qt5_add_resources(LIBMSM_SRCS ${LIBMSM_RESOURCES})

add_library (msm STATIC ${LIBMSM_SRCS})
target_link_libraries (msm
    Qt5::Core
    Qt5::Widgets
    KF5::Auth)
target_include_directories (msm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
