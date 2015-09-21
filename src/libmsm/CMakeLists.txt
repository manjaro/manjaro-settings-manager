set(LIBMSM_SRCS
    ActionDialog.cpp
    Kernel.cpp
    KernelModel.cpp
    LanguageCommon.cpp)

add_library (msm STATIC ${LIBMSM_SRCS})
target_link_libraries (msm KF5::ConfigWidgets
    KF5::CoreAddons)
target_include_directories (msm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
