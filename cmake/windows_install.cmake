# ==============================================================================
# Windows-specific packaging logic for Qt6
# ==============================================================================
if(WIN32)
    # 前提：你需要在这段脚本之前已经调用了 find_package(Qt6 ...)，例如：
    # find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
    # Qt6::windeployqt 工具目标会自动被找到

    # 1. 使用 CMAKE_BUILD_TYPE 使名称动态化 (这部分无需修改)
    set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/TrayMusicPlayer_${CMAKE_BUILD_TYPE}")
    set(PACKAGE_ZIP_NAME "TrayMusicPlayer_Windows_Portable_v${PROJECT_VERSION}_${CMAKE_BUILD_TYPE}.zip")

    # 获取目标可执行文件的最终名称 (这部分无需修改)
    get_target_property(FINAL_EXE_NAME_BASE tray_music_app OUTPUT_NAME)
    if(NOT FINAL_EXE_NAME_BASE)
        set(FINAL_EXE_NAME_BASE "musicplayer") # 提供一个备用名称
    endif()
    set(EXE_FILE "${FINAL_EXE_NAME_BASE}.exe")

    # 定义安装规则，告诉 `cmake --install` 要做什么 (这部分无需修改)
    install(TARGETS tray_music_app
            RUNTIME DESTINATION bin  # 安装到 <prefix>/bin 目录
    )
    install(TARGETS tray_launcher
            RUNTIME DESTINATION .    # 安装到 <prefix> 根目录
    )

    # 2. 创建一个不属于 ALL 的、可选的打包目标 (这部分无需修改)
    add_custom_target(package
            # 确保在打包前，两个可执行文件已经编译完成
            DEPENDS tray_music_app tray_launcher
            COMMENT "Creates a distributable package for the application."
    )

    add_custom_command(
            TARGET package
            PRE_BUILD # 只包含 PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${PACKAGE_DIR}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}"
            COMMENT "Preparing package directory..." # 只有一个 COMMENT
    )

    add_custom_command(
            TARGET package
            POST_BUILD # 只包含 POST_BUILD
            COMMAND ${CMAKE_COMMAND} --install . --prefix "${PACKAGE_DIR}" --config ${CMAKE_BUILD_TYPE}
            COMMAND "$<TARGET_FILE:Qt6::windeployqt>"
            --dir "${PACKAGE_DIR}/bin"
            "${PACKAGE_DIR}/bin/${EXE_FILE}"
            COMMAND powershell.exe -ExecutionPolicy Bypass -Command "Compress-Archive -Path '${PACKAGE_DIR}/*' -DestinationPath '${CMAKE_BINARY_DIR}/${PACKAGE_ZIP_NAME}' -Force"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            COMMENT "Creating final ZIP archive: ${PACKAGE_ZIP_NAME}" # 只有一个 COMMENT
    )
endif()

# 为了方便，可以创建一个别名，比如 package_release
# add_custom_target(package_release DEPENDS package) # 如果你仍然想用这个名字
