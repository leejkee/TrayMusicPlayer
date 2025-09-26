cmake_minimum_required(VERSION 3.20)

add_library(tray_music_log INTERFACE)
target_include_directories(tray_music_log INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(tray_music_log INTERFACE cxx_std_17)
