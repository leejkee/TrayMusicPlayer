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
set(TRAY_TEST ON)

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

# =========================================================================
# 部署和打包逻辑 (Windows Release ZIP)
# =========================================================================

# --- 1. 定义变量和路径 ---

# 1.1 获取 windeployqt 的路径
# 这依赖于 Qt6::Core 库的实际位置
get_target_property(QT_BIN_DIR Qt6::Core INTERFACE_IMPORT_LOCATION)
string(REPLACE "/Qt6Core.dll" "" QT_BIN_DIR ${QT_BIN_DIR})
#message(STATUS "qt_bin: ${QT_BIN_DIR}")
set(WINDEPLOYQT_EXECUTABLE "${QT_BIN_DIR}/windeployqt.exe")
#message(STATUS "windeployqt_bin: ${WINDEPLOYQT_EXECUTABLE}")

# 1.2 定义最终的发布路径和压缩包名称
set(PACKAGE_DIR "${CMAKE_BINARY_DIR}/TrayMusicPlayer_Release")
set(PACKAGE_ZIP_NAME "TrayMusicPlayer_Windows_Portable_v${PROJECT_VERSION}.zip")

get_target_property(FINAL_EXE_NAME_BASE tray_music_app OUTPUT_NAME)
if(NOT FINAL_EXE_NAME_BASE)
    set(FINAL_EXE_NAME_BASE "TrayMusic")
endif()
#message(STATUS "Final EXE Base Name (Output Name or Target Name): ${FINAL_EXE_NAME_BASE}")

if (WIN32)
    set(EXE_FILE "${FINAL_EXE_NAME_BASE}.exe")
elseif (UNIX)
    set(EXE_FILE ${FINAL_EXE_NAME_BASE})
endif ()

# --- 2. 编写 install() 规则 (定义目录结构) ---
# all these rules determine how 'cmake --install' works

# 2.1 安装主可执行文件到发布根目录
install(TARGETS tray_music_app
        RUNTIME DESTINATION bin
)

# 2.2 安装配置文件到 settings/ 目录
install(DIRECTORY ${CMAKE_SOURCE_DIR}/settings/
        DESTINATION settings/
        FILES_MATCHING PATTERN "*.json"
)

# --- 3. 创建 package_release 自定义目标 (驱动打包流程) ---

add_custom_target(package_release ALL
        # 确保在打包前，应用已经被成功构建 (Release配置)
        DEPENDS tray_music_app
)

# A. 清理和创建目标目录 (在构建前执行)
add_custom_command(TARGET package_release PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${PACKAGE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_DIR}/bin"
        COMMENT "Preparing package directory structure."
)

# B. 运行 CMake INSTALL (将 EXE, Configs, start.bat 复制到 ${PACKAGE_DIR})
add_custom_command(TARGET package_release POST_BUILD
        COMMAND ${CMAKE_COMMAND} --install . --prefix "${PACKAGE_DIR}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running CMake install to copy EXE and resources."
)

# C. 运行 windeployqt (部署 DLLs 到 bin/ 目录)
add_custom_command(TARGET package_release POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE}
        "${PACKAGE_DIR}/${EXE_FILE}" # 目标是已安装的EXE
        --dir "${PACKAGE_DIR}/bin"  # 关键参数：指定 DLLs 的输出目录
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Running windeployqt to collect Qt dependencies into bin/."
)

# D. 压缩最终目录 (创建 ZIP 文件)
add_custom_command(TARGET package_release POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Compressing ${PACKAGE_DIR}..."
        # 使用 PowerShell 创建 ZIP 压缩包 (Win10/11 默认支持)
        COMMAND powershell.exe -Command
        "Compress-Archive -Path '${PACKAGE_DIR}/*' -DestinationPath '${CMAKE_BINARY_DIR}/${PACKAGE_ZIP_NAME}' -Force"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        COMMENT "Creating final ZIP archive at ${PACKAGE_ZIP_NAME}."
)