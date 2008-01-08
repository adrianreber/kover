project(getcover)

include(FindPkgConfig)

set(VERSION 1)
set(PACKAGE ${PROJECT_NAME})

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

pkg_check_modules(BLA libcurl)
pkg_check_modules(BLU libxml-2.0)
pkg_check_modules(BLO glib-2.0)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.c)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${BLA_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${BLU_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${BLO_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${BLA_LDFLAGS})
target_link_libraries(${PROJECT_NAME} ${BLU_LDFLAGS})
target_link_libraries(${PROJECT_NAME} ${BLO_LDFLAGS})
