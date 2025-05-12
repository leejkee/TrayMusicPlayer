# 最小 CMake 版本要求
cmake_minimum_required(VERSION 3.20) # ExternalProject 通常需要较新版本

project(BuildTagLibExternally C CXX)

include(ExternalProject)

set(TAGLIB_INSTALL_DIR ${CMAKE_SOURCE_DIR}/taglib_lib)

set(TAGLIB_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../3rdParty/taglib")

file(MAKE_DIRECTORY ${TAGLIB_INSTALL_DIR})

ExternalProject_Add(
        TagLib_lib # 外部项目的目标名称
        SOURCE_DIR "${TAGLIB_SOURCE_DIR}" # 使用上面定义的源代码目录
        BINARY_DIR "${CMAKE_BINARY_DIR}/taglib_build" # TagLib 的构建目录，位于本项目的构建目录下
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${TAGLIB_INSTALL_DIR} # 指定安装路径
        -DBUILD_SHARED_LIBS=ON
        -DBUILD_BINDINGS=OFF
        -DBUILD_EXAMPLES=OFF
        -DBUILD_TESTING=OFF
        -DVISIBILITY_HIDDEN=ON
        -DWITH_ZLIB=OFF
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_CXX_STANDARD=17
        -G "Ninja"
        # 传递当前项目的编译器信息给外部项目
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM} # 指定构建工具程序
        BUILD_COMMAND ${CMAKE_COMMAND} --build . --config Release # 构建命令
        INSTALL_COMMAND ${CMAKE_COMMAND} --install . --config Release --strip # 安装命令
        UPDATE_COMMAND ""
)
