qt_add_executable(TrayMusicPlayer
        MANUAL_FINALIZATION
        main.cpp
        trayapp.h
        trayapp.cpp
        windowmanager.cpp
        windowmanager.h
)

target_link_libraries(TrayMusicPlayer PRIVATE
        tray_music_ui
        tray_music_core
        tray_qrc
)


if(WIN32)
    set_target_properties(TrayMusicPlayer PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

qt_finalize_target(TrayMusicPlayer)