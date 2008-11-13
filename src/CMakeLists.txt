include(FindPkgConfig)

set(${PROJECT_NAME}_SRCS
   main.cpp
   kover_config.cc
   pd.cpp
   sd.cpp
   server.cpp
   cddb_fill.cc
   pa.cpp
   categories.cc
   cddb_211_item.cc
   inexact_dialog.cc
   koverfile.cpp
   cdview.cc
   kovertop.cc
   id.cc 
   directory.cc
   without_cd.cc
)

pkg_check_modules(CDIO REQUIRED libcdio)
pkg_check_modules(CDDB REQUIRED libcddb)
set(CMAKE_C_FLAGS "-g -Wall")
include_directories(${CDIO_INCLUDE_DIRS})
include_directories(${CDDB_INCLUDE_DIRS})

kde4_add_executable(${PROJECT_NAME} ${kover_SRCS})

target_link_libraries(${PROJECT_NAME} ${KDE4_KDEUI_LIBS})
target_link_libraries(${PROJECT_NAME} ${KDE4_KFILE_LIBS})
target_link_libraries(${PROJECT_NAME} ${CDIO_LDFLAGS})
target_link_libraries(${PROJECT_NAME} ${CDDB_LDFLAGS})

install(TARGETS kover DESTINATION ${BIN_INSTALL_DIR} )
install(FILES kover.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install(FILES kover.xml  DESTINATION  ${XDG_MIME_INSTALL_DIR} )
install(FILES koverui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kover )

add_subdirectory(icons)
