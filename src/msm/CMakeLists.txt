include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_GLOB
     msm.desktop
     qml/main.qml)

set(MSM_SRCS
    ${MSM_GLOB}
    main.cpp
    ModuleView.cpp
    MsmApplication.cpp
    MsmWindow.cpp)

set(MSM_RESOURCES
    msm.qrc)
QT5_ADD_RESOURCES (MSM_SRCS ${MSM_RESOURCES})

add_executable(msm_bin ${MSM_SRCS})
SET_TARGET_PROPERTIES(msm_bin
    PROPERTIES
        AUTOMOC TRUE
        ENABLE_EXPORTS TRUE
        RUNTIME_OUTPUT_NAME msm)
target_link_libraries(msm_bin
    Qt5::Core
    Qt5::Widgets
    Qt5::Quick
    KF5::ConfigWidgets
    KF5::KCMUtils
    KF5::IconThemes)

install( TARGETS msm_bin
    # BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES msm.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
