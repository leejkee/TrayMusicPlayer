cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
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

include(CTest)
enable_testing()
add_subdirectory(src)
add_subdirectory(3rdParty)

if (TRAY_TEST)
    add_subdirectory(test)
endif ()

get_target_property(QT_BIN_DIR Qt6::Core INTERFACE_IMPORT_LOCATION)
string(REPLACE "/Qt6Core.dll" "" QT_BIN_DIR ${QT_BIN_DIR})
#message(STATUS "qt_bin: ${QT_BIN_DIR}")
set(WINDEPLOYQT_EXECUTABLE "${QT_BIN_DIR}/windeployqt.exe")
#message(STATUS "windeployqt_bin: ${WINDEPLOYQT_EXECUTABLE}")

set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/TrayMusicPlayer_Release")
set(PACKAGE_ZIP_NAME "TrayMusicPlayer_Windows_Portable_v${PROJECT_VERSION}.zip")

get_target_property(FINAL_EXE_NAME_BASE tray_music_app OUTPUT_NAME)
if(NOT FINAL_EXE_NAME_BASE)
    set(FINAL_EXE_NAME_BASE "musicplayer")
endif()
#message(STATUS "Final EXE Base Name (Output Name or Target Name): ${FINAL_EXE_NAME_BASE}")

if (WIN32)
    set(EXE_FILE "${FINAL_EXE_NAME_BASE}.exe")
elseif (UNIX)
    set(EXE_FILE ${FINAL_EXE_NAME_BASE})
endif ()


install(TARGETS tray_music_app
        RUNTIME DESTINATION bin
)

if (WIN32)
    install(TARGETS tray_launcher
            RUNTIME DESTINATION .
    )
endif ()

if (WIN32)
        add_custom_target(package_release ALL
                DEPENDS tray_music_app tray_launcher
        )
else ()
        add_custom_target(package_release ALL
                DEPENDS tray_music_app
        )
endif ()

add_custom_command(TARGET package_release PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${PACKAGE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/bin"
        COMMENT "Preparing package directory structure."
)

add_custom_command(TARGET package_release POST_BUILD
        COMMAND ${CMAKE_COMMAND} --install . --prefix "${PACKAGE_DIR}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running CMake install to copy EXE and resources."
)

add_custom_command(TARGET package_release POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE}
        "${PACKAGE_DIR}/bin/${EXE_FILE}"
        --dir "${PACKAGE_DIR}/bin"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running windeployqt to collect Qt dependencies into bin/."
)

add_custom_command(TARGET package_release POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Compressing ${PACKAGE_DIR}..."
        COMMAND powershell.exe -Command
        "Compress-Archive -Path '${PACKAGE_DIR}/*' -DestinationPath '${CMAKE_BINARY_DIR}/${PACKAGE_ZIP_NAME}' -Force"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Creating final ZIP archive at ${PACKAGE_ZIP_NAME}."
)