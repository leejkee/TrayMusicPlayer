cmake_minimum_required(VERSION 3.20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

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
        OUTPUT_NAME "traymusic"
)


if (WIN32)
    set_target_properties(tray_music_app PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif ()

qt_finalize_target(tray_music_app)

if (WIN32)
    add_executable(tray_launcher WIN32 launcher_win32.cpp)
    target_link_options(tray_launcher PRIVATE -municode)
    target_link_libraries(tray_launcher PRIVATE shlwapi)
    set_target_properties(tray_launcher PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
    set_target_properties(tray_launcher PROPERTIES
            OUTPUT_NAME "TrayMusicLauncher"
    )
    target_compile_features(tray_launcher PRIVATE cxx_std_17)
endif ()