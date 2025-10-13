# ==============================================================================
# Windows-specific packaging logic for Qt6
# ==============================================================================
if(WIN32)

    set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/TrayMusicPlayer_${CMAKE_BUILD_TYPE}")
    set(PACKAGE_ZIP_NAME "TrayMusicPlayer_Windows_Portable_v${PROJECT_VERSION}_${CMAKE_BUILD_TYPE}.zip")

    get_target_property(FINAL_EXE_NAME_BASE tray_music_app OUTPUT_NAME)
    if(NOT FINAL_EXE_NAME_BASE)
        set(FINAL_EXE_NAME_BASE "traymusic") # 提供一个备用名称
    endif()
    set(EXE_FILE "${FINAL_EXE_NAME_BASE}.exe")

    install(TARGETS tray_music_app
            RUNTIME DESTINATION bin
    )
    install(TARGETS tray_launcher
            RUNTIME DESTINATION .
    )

    add_custom_target(package
            DEPENDS tray_music_app tray_launcher
            COMMENT "Creates a distributable package for the application."
    )

    add_custom_command(
            TARGET package
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${PACKAGE_DIR}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}"
            COMMENT "Preparing package directory..."
    )

    add_custom_command(
            TARGET package
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} --install . --prefix "${PACKAGE_DIR}" --config ${CMAKE_BUILD_TYPE}
            COMMAND "$<TARGET_FILE:Qt6::windeployqt>"
            --dir "${PACKAGE_DIR}/bin"
            "${PACKAGE_DIR}/bin/${EXE_FILE}"
            COMMAND powershell.exe -ExecutionPolicy Bypass -Command "Compress-Archive -Path '${PACKAGE_DIR}/*' -DestinationPath '${CMAKE_BINARY_DIR}/${PACKAGE_ZIP_NAME}' -Force"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Creating final ZIP archive: ${PACKAGE_ZIP_NAME}"
    )
endif()