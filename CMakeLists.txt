project(kover)
find_package(KDE4 REQUIRED)
include (KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(K_VERSION 5)
set(K_PACKAGE ${PROJECT_NAME})

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)


add_subdirectory(src)
add_subdirectory(po)

install(FILES kover.1  DESTINATION  ${MAN_INSTALL_DIR}/man1 )

SET(CPACK_BINARY_BUNDLE "OFF")
SET(CPACK_SOURCE_GENERATOR "TBZ2")
SET(CPACK_PACKAGE_VERSION ${VERSION})
SET(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-${VERSION})
SET(CPACK_SOURCE_IGNORE_FILES ${CMAKE_CURRENT_BINARY_DIR} "/.git/")

INCLUDE(CPack)

