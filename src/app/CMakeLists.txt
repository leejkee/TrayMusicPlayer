cmake_minimum_required(VERSION 3.20)

qt_add_executable(tray_music_app
        MANUAL_FINALIZATION
        main.cpp
        trayapp.cpp
        trayapp.h
)

target_link_libraries(tray_music_app PRIVATE
        tray_music_ui
        tray_music_core
        tray_qrc
        tray_music_log
)

target_compile_features(tray_music_app PRIVATE cxx_std_17)

set_property(TARGET tray_music_app PROPERTY AUTOMOC ON)

set_target_properties(tray_music_app PROPERTIES
        OUTPUT_NAME "TrayMusic"
)

if(WIN32)
    set_target_properties(TrayMusicPlayer PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

qt_finalize_target(tray_music_app)

if (WIN32)
    set_target_properties(tray_launcher PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
    add_executable(tray_launcher launcher_win32.cpp)
    target_compile_features(tray_launcher PRIVATE cxx_std_17)
endif ()