set(kover_SRCS
   kover.cpp
   main.cpp
   koverview.cpp
 )

kde4_add_ui_files(kover_SRCS koverview_base.ui prefs_base.ui)
kde4_add_kcfg_files(kover_SRCS settings.kcfgc )
kde4_add_executable(kover ${kover_SRCS})

target_link_libraries(kover ${KDE4_KDEUI_LIBS} )

install(TARGETS kover DESTINATION ${BIN_INSTALL_DIR} )
install( FILES kover.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kover.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES koverui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kover )
