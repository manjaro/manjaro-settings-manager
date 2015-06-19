include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB MSM_NOTIFIER_GLOB
     msm_notifier.desktop
     resources/language.png
     resources/tux-manjaro.png)

set(MSM_NOTIFIER_SRCS
    ${MSM_NOTIFIER_GLOB}
    Notifier.cpp
    NotifierApp.cpp
    global.cpp
    ../modules/kernel/Kernel.cpp
    ../modules/kernel/KernelModel.cpp)

#set(MSM_NOTIFIER_UI)
#ki18n_wrap_ui(MSM_NOTIFIER_SRCS ${MSM_NOTIFIER_UI})

set(MSM_NOTIFIER_RESOURCES
    notifier.qrc)
QT5_ADD_RESOURCES (MSM_NOTIFIER_SRCS ${MSM_NOTIFIER_RESOURCES})

add_executable(msm_notifier ${MSM_NOTIFIER_SRCS})
target_compile_definitions(msm_notifier PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_notifier
    Qt5::Widgets
    KF5::Notifications)

install(TARGETS msm_notifier DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES msm_notifier.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)

#install(FILES msm_notifier.notifyrc DESTINATION ${KNOTIFYRC_INSTALL_DIR})
