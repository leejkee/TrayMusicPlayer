
qt_add_executable(TrayMusicPlayer
        MANUAL_FINALIZATION
        main.cpp
)
target_link_libraries(TrayMusicPlayer PUBLIC tray_music_ui)
if(WIN32)
    set_target_properties(TrayMusicPlayer PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

qt_finalize_target(TrayMusicPlayer)
