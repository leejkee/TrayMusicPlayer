cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE WIDGETS_SRC CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(tray_music_widgets STATIC ${WIDGETS_SRC})

target_include_directories(tray_music_widgets PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_property(TARGET tray_music_widgets PROPERTY AUTOMOC ON)

target_link_libraries(tray_music_widgets PRIVATE
        Qt6::Widgets
        Qt6::Core
        Qt6::SvgWidgets
        tray_qrc
        tray_music_log
)
