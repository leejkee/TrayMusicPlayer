cmake_minimum_required(VERSION 3.20)

find_package(TagLib REQUIRED)

set(CORE_SOURCES
        include/core.h
        core.cpp
        databasemanager/databasemanager.h
        databasemanager/databasemanager.cpp
        listcache/listcache.h
        listcache/listcache.cpp
        player/player.h
        player/player.cpp
        playlist/playlist.h
        playlist/playlist.cpp
        settings/settings.h
        settings/settings.cpp
        common/song.h
        common/song.cpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}/settings
)


target_link_libraries(tray_music_core PRIVATE
        Qt6::Multimedia
        Qt6::Sql
        Tray::tray_log
        TagLib::tag
)

if (TRAY_TEST)
    add_library(tray_database_for_Test STATIC
            databasemanager/DatabaseManager.h
            databasemanager/DatabaseManager.cpp
            common/Song.cpp
            common/Song.h
    )

    target_link_libraries(tray_database_for_Test PRIVATE
            Qt6::Sql
            Tray::tray_log
    )

    target_include_directories(tray_database_for_Test PUBLIC
            databasemanager
    )
endif ()