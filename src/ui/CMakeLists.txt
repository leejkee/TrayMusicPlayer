cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(tray_music_ui STATIC ${UI_SOURCES})

target_include_directories(tray_music_ui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tray_music_ui PUBLIC
        Qt6::Widgets
        Qt6::Core
        tray_qrc
        tray_log
        tray_music_widgets
)