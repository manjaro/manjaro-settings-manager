cmake_policy(SET CMP0063 NEW)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB NOTIFIER_GLOB
     msm_notifier.desktop
     icons/language.png
     icons/tux-manjaro.png)

set(NOTIFIER_SRCS
    ${NOTIFIER_GLOB}
    main.cpp
    Notifier.cpp
    NotifierApp.cpp)

set(NOTIFIER_RESOURCES
    notifier.qrc)
QT5_ADD_RESOURCES (NOTIFIER_SRCS ${NOTIFIER_RESOURCES})

add_executable(msm_notifier ${NOTIFIER_SRCS})
target_compile_definitions(msm_notifier PRIVATE -DPROJECT_VERSION="${PROJECT_VERSION}")
target_link_libraries(msm_notifier
    msm
    Qt5::Widgets)

install(TARGETS msm_notifier DESTINATION ${CMAKE_INSTALL_BINDIR})
install(PROGRAMS msm_notifier.desktop DESTINATION ${KDE_INSTALL_AUTOSTARTDIR})
