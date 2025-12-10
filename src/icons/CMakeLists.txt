# Install hicolor theme icons
ecm_install_icons(ICONS
    16-apps-audio_mode.png
    32-apps-kover.png
    48-apps-kover.png
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)

# Install kover-specific icons to app data directory
install(FILES
    kover_back_content.png
    kover_back_title_content.png
    kover_cdaudio.png
    kover_front_title-content-right_content-left.png
    kover_front_title_only.png
    kover_front_title-right_content-left.png
    kover_one_page.png
    DESTINATION ${KDE_INSTALL_ICONDIR}
)
