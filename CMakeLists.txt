cmake_minimum_required(VERSION 3.16)

project(kover VERSION 7)

set(QT_MIN_VERSION "6.4.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(KDE_INSTALL_SBINDIR "sbin")

set(KDE_INSTALL_DIRS_NO_DEPRECATED TRUE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FeatureSummary)
include(ECMInstallIcons)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    PrintSupport
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    I18n
    XmlGui
    KIO
    ConfigWidgets
    IconThemes
)

set(K_VERSION "7")
set(K_PACKAGE ${PROJECT_NAME})

configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(src)
add_subdirectory(po)

install(FILES kover.1 DESTINATION ${KDE_INSTALL_MANDIR}/man1)

set(CPACK_BINARY_BUNDLE "OFF")
set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION ${K_VERSION})
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PROJECT_NAME}-${K_VERSION})
set(CPACK_SOURCE_IGNORE_FILES ${CMAKE_CURRENT_BINARY_DIR} "/.git/" "/getcover/" "/.gitignore")

include(CPack)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

