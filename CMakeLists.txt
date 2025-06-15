cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(TrayMusicPlayer
        VERSION 0.1
        DESCRIPTION "Cross-platform Music Player with Core-UI Separation"
        LANGUAGES CXX
)

#option(TRAY_TEST "Build the Test module" ON)
set(TRAY_TEST OFF)

find_package(Qt6 REQUIRED COMPONENTS
    Multimedia
    Core
    Sql
    Widgets
    SvgWidgets
    Test
)
add_subdirectory(tools)
add_subdirectory(core)
add_subdirectory(ui)
add_subdirectory(app)
add_subdirectory(resource)

if (TRAY_TEST)
    add_subdirectory(test)
endif ()
