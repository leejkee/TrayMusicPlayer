cmake_minimum_required(VERSION 3.16)

project(TrayMusicPlayer 
    VERSION 0.1 
    DESCRIPTION "Cross-platform Music Player with Core-UI Separation"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

find_package(Qt6 REQUIRED COMPONENTS
    Core 
    Widgets 
    Multimedia 
    Sql
    SvgWidgets
)


set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(RESOURCES_DIR res)
set(THIRD_PARTY_DIR 3rdParty)

file(GLOB_RECURSE CORE_SOURCES "${SOURCE_DIR}/core/*.cpp")
add_library(tray_music_core STATIC ${CORE_SOURCES})
target_include_directories(tray_music_core PUBLIC
    ${INCLUDE_DIR}/core
)
target_link_libraries(tray_music_core PUBLIC
    Qt6::Core
    Qt6::Multimedia
    Qt6::Sql
)


add_library(tray_music_interface INTERFACE
        include/core/interface/IAudioBackend.h)
target_include_directories(tray_music_interface INTERFACE
    ${INCLUDE_DIR}/interface
)
target_link_libraries(tray_music_interface INTERFACE
    tray_music_core
)

# main
file(GLOB_RECURSE UI_SOURCES "${SOURCE_DIR}/ui/*.cpp")
file(GLOB_RECURSE UI_HEADERS "${INCLUDE_DIR}/ui/*.h")
qt_add_executable(TrayMusicPlayer
    ${SOURCE_DIR}/main.cpp
    ${UI_SOURCES}
    ${UI_HEADERS}
    ${RESOURCES_DIR}/systray.qrc
)
target_include_directories(TrayMusicPlayer PRIVATE
    ${INCLUDE_DIR}/ui
    ${INCLUDE_DIR}/core
    ${INCLUDE_DIR}/interface
)
target_link_libraries(TrayMusicPlayer PRIVATE
    tray_music_interface
    Qt6::Widgets
    Qt6::SvgWidgets
)

if(WIN32)
    set_target_properties(TrayMusicPlayer PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
