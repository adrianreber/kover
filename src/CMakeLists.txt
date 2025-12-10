include(FindPkgConfig)

set(kover_SRCS
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

pkg_check_modules(CDIO REQUIRED libcdio>=0.90)
pkg_check_modules(CDDB REQUIRED libcddb)

add_executable(kover ${kover_SRCS})

target_include_directories(kover PRIVATE ${CDIO_INCLUDE_DIRS} ${CDDB_INCLUDE_DIRS})

target_link_libraries(kover
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
    KF6::CoreAddons
    KF6::I18n
    KF6::XmlGui
    KF6::KIOCore
    KF6::KIOWidgets
    KF6::ConfigWidgets
    KF6::IconThemes
    ${CDIO_LDFLAGS}
    ${CDDB_LDFLAGS}
)

install(TARGETS kover ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kover.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES kover.xml DESTINATION ${KDE_INSTALL_MIMEDIR})
install(FILES koverui.rc DESTINATION ${KDE_INSTALL_KXMLGUIDIR}/kover)

add_subdirectory(icons)
