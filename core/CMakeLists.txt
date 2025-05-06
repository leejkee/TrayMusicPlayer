cmake_minimum_required(VERSION 3.20)


set(CORE_SOURCES
        include/Core.h
        common/Song.cpp
        databasemanager/DatabaseManager.cpp
        listcache/ListCache.cpp
        player/Player.cpp
        playlist/PlayList.cpp
        Core.cpp
)

add_library(tray_music_core STATIC ${CORE_SOURCES})

target_compile_features(tray_music_core PRIVATE cxx_std_17)

target_include_directories(tray_music_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(tray_music_core PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/listcache
        ${CMAKE_CURRENT_SOURCE_DIR}/player
        ${CMAKE_CURRENT_SOURCE_DIR}/playlist
        ${CMAKE_CURRENT_SOURCE_DIR}/databasemanager
        ${CMAKE_CURRENT_SOURCE_DIR}/common
)


target_link_libraries(tray_music_core PRIVATE
        Qt6::Multimedia
        TagLib::tag
        Qt6::Sql
        Tray::tray_log
)


add_library(tray_config STATIC Settings.cpp)
target_include_directories(tray_config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(tray_config PRIVATE
        Tray::tray_log
)