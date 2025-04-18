cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
        include/*.h
        src/*.cpp
)

add_library(tray_music_core STATIC ${CORE_SOURCES})

target_compile_features(tray_music_core PRIVATE cxx_std_17)

target_include_directories(tray_music_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/interfaces
        ${CMAKE_CURRENT_SOURCE_DIR}/include/services
)

target_include_directories(tray_music_core PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/engine
)

target_link_libraries(tray_music_core PUBLIC
        Qt6::Multimedia
        TagLib::tag
        Qt6::Sql
)
