cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(tray_music_core STATIC ${CORE_SRC})

target_compile_features(tray_music_core PRIVATE cxx_std_17)

target_include_directories(tray_music_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(tray_music_core PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_property(TARGET tray_music_core PROPERTY AUTOMOC ON)

target_link_libraries(tray_music_core PRIVATE
        Qt6::Multimedia
        Qt6::Sql
        lyric_parser
        tray_music_log
)
