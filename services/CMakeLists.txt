cmake_minimum_required(VERSION 3.20)
add_library(tray_music_services STATIC

)

target_include_directories(tray_music_services PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/interfaces
)

target_link_libraries(tray_music_services PRIVATE
        tray_music_core
        Qt6::Sql
)