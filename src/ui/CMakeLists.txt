cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(tray_music_ui STATIC ${UI_SOURCES})

target_compile_features(tray_music_ui PRIVATE cxx_std_17)

set_property(TARGET tray_music_ui PROPERTY AUTOMOC ON)

target_include_directories(tray_music_ui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tray_music_ui PUBLIC
        Qt6::Widgets
        Qt6::Core
        tray_qrc
        Qt6::SvgWidgets
        tray_music_log
        tray_music_widgets
)