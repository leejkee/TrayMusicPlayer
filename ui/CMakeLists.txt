cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE UI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc
)

qt_add_executable(TrayMusicPlayer
        MANUAL_FINALIZATION
        ${UI_SOURCES}
)

target_link_libraries(TrayMusicPlayer PUBLIC
#        tray_music_core
        Qt6::Widgets
        Qt6::Svg
        Qt6::Core
)

target_include_directories(TrayMusicPlayer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(WIN32)
    set_target_properties(TrayMusicPlayer PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

qt_finalize_target(TrayMusicPlayer)
