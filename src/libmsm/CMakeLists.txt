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
    LanguagePackagesCommon.cpp
    LanguagePackagesItem.cpp

    ${KDSINGLEAPPLICATIONGUARD})

cmake_policy(SET CMP0063 NEW)

add_library (msm STATIC ${LIBMSM_SRCS})
target_link_libraries (msm KF5::ConfigWidgets
    KF5::CoreAddons)
target_include_directories (msm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
