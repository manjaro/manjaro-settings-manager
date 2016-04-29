cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB NOTIFIER_KDE_GLOB
     msm_notifier.desktop
     icons/language.png
     icons/tux-manjaro.png)

set(NOTIFIER_KDE_SRCS
    ${NOTIFIER_KDE_GLOB}
    main.cpp
    Notifier.cpp
    NotifierApp.cpp)

set(NOTIFIER_KDE_RESOURCES
    notifier.qrc)
QT5_ADD_RESOURCES (NOTIFIER_KDE_SRCS ${NOTIFIER_KDE_RESOURCES})

add_executable(msm_kde_notifier ${NOTIFIER_KDE_SRCS})
target_compile_definitions(msm_kde_notifier PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_kde_notifier
    msm
    Qt5::Widgets
    KF5::Notifications)

install(TARGETS msm_kde_notifier DESTINATION ${CMAKE_INSTALL_BINDIR})
install(PROGRAMS msm_kde_notifier.desktop DESTINATION ${KDE_INSTALL_AUTOSTARTDIR})
