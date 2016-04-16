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
    ActionDialog.cpp
    Kernel.cpp
    KernelModel.cpp
    LanguageCommon.cpp
    LanguagePackages.cpp
    LanguagePackagesItem.cpp
    ListWidget.cpp
    PageWidget.cpp
    ApplyDialog.cpp

    ${KDSINGLEAPPLICATIONGUARD})

set(LIBMSM_UI
    ApplyDialog.ui)
qt5_wrap_ui(LIBMSM_SRCS ${LIBMSM_UI})

set(LIBMSM_RESOURCES
    language_packages.qrc
    translations.qrc)
QT5_ADD_RESOURCES (LIBMSM_SRCS ${LIBMSM_RESOURCES})

cmake_policy(SET CMP0063 NEW)

add_library (msm STATIC ${LIBMSM_SRCS})
target_link_libraries (msm
    KF5::ConfigWidgets)
target_include_directories (msm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
