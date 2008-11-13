project(kover)
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(VERSION 4)
set(PACKAGE ${PROJECT_NAME})

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(src)

install(FILES kover.1  DESTINATION  ${MAN_INSTALL_DIR}/man1 )
