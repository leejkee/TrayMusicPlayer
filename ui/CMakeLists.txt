cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/res/*.qrc
)

add_library(tray_music_ui STATIC ${UI_SOURCES})



target_link_libraries(tray_music_ui PUBLIC
        tray_music_core
        Qt6::Widgets
        Qt6::Core
        Qt6::SvgWidgets
)

target_include_directories(tray_music_ui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
