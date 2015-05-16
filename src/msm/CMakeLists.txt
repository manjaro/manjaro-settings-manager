include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(MSM_SRCS
    main.cpp
    ModuleView.cpp
    MsmApplication.cpp
    MsmWindow.cpp)

add_executable(msm_bin ${MSM_SRCS})
SET_TARGET_PROPERTIES(msm_bin
    PROPERTIES
        AUTOMOC TRUE
        ENABLE_EXPORTS TRUE
        RUNTIME_OUTPUT_NAME msm)
target_link_libraries(msm_bin
    Qt5::Core
    Qt5::Widgets
    KF5::ConfigWidgets
    KF5::KCMUtils
    KF5::I18n
    KF5::IconThemes)

install( TARGETS msm_bin
    # BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES msm.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
