include(FindPkgConfig)

set(${PROJECT_NAME}_SRCS
   kover.cpp
   main.cpp
   koverview.cpp
   config.cc
   pd.cpp
 )

pkg_check_modules(CDIO REQUIRED libcdio)
pkg_check_modules(CDDB REQUIRED libcddb)
set(CMAKE_C_FLAGS "-g -Wall")
include_directories(${CDIO_INCLUDE_DIRS})
include_directories(${CDDB_INCLUDE_DIRS})

kde4_add_ui_files(kover_SRCS koverview_base.ui prefs_base.ui)
kde4_add_kcfg_files(kover_SRCS settings.kcfgc )
kde4_add_executable(${PROJECT_NAME} ${kover_SRCS})

target_link_libraries(${PROJECT_NAME} ${KDE4_KDEUI_LIBS})
target_link_libraries(${PROJECT_NAME} ${CDIO_LDFLAGS})
target_link_libraries(${PROJECT_NAME} ${CDDB_LDFLAGS})

install(TARGETS kover DESTINATION ${BIN_INSTALL_DIR} )
install( FILES kover.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kover.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES koverui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kover )
