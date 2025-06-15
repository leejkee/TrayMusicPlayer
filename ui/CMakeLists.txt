cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/panels/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/panels/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/common/uiconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/common/uimetadata.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

add_library(tray_music_ui STATIC ${UI_SOURCES})

if (TRAY_TEST)
    message(STATUS "Test mode ready")
    target_include_directories(tray_music_ui PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/panels
            ${CMAKE_CURRENT_SOURCE_DIR}/common
    )
else ()
    message(STATUS "No Test ready")
    target_include_directories(tray_music_ui PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif ()


target_include_directories(tray_music_ui PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/panels
)

target_link_libraries(tray_music_ui PUBLIC
        Qt6::Widgets
        Qt6::Core
        Qt6::SvgWidgets
        tray_qrc
        tray_log
)