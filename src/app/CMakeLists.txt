cmake_minimum_required(VERSION 3.20)

qt_add_executable(tray_music_app
        MANUAL_FINALIZATION
        main.cpp
        coreservice.cpp
        coreservice.h
)

target_link_libraries(tray_music_app PRIVATE
        tray_music_ui
        tray_music_core
        tray_qrc
)

set_property(TARGET tray_music_app PROPERTY AUTOMOC ON)

# if(WIN32)
#     set_target_properties(TrayMusicPlayer PROPERTIES
#             WIN32_EXECUTABLE TRUE
#     )
# endif()

qt_finalize_target(tray_music_app)