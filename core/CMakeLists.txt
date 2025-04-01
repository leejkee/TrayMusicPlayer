cmake_minimum_required(VERSION 3.20)
file(GLOB_RECURSE CORE_SOURCES
        include/*.h
        src/*.cpp
        core.qrc
)

add_library(tray_music_core STATIC ${CORE_SOURCES})

target_include_directories(tray_music_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/interfaces
)

target_include_directories(tray_music_core PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/include/engine
)

target_link_libraries(tray_music_core PUBLIC
        Qt6::Multimedia
)
