cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/WindowManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/panels/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/panels/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/music_list_widget/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/music_list_widget/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/player_widget/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/player_widget/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/settings_widget/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/settings_widget/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/top_bar_widget/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/top_bar_widget/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/view_widget/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/view_widget/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/common/UiConfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qss.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/svg.qrc
)

add_library(tray_music_ui STATIC ${UI_SOURCES})

target_include_directories(tray_music_ui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(tray_music_ui PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/panels
        ${CMAKE_CURRENT_SOURCE_DIR}/music_list_widget
        ${CMAKE_CURRENT_SOURCE_DIR}/player_widget
        ${CMAKE_CURRENT_SOURCE_DIR}/settings_widget
        ${CMAKE_CURRENT_SOURCE_DIR}/top_bar_widget
        ${CMAKE_CURRENT_SOURCE_DIR}/view_widget
        ${CMAKE_CURRENT_SOURCE_DIR}/common
)

target_link_libraries(tray_music_ui PUBLIC
        Qt6::Widgets
        Qt6::Core
        Qt6::SvgWidgets
        tray_qrc
        tray_log
)
