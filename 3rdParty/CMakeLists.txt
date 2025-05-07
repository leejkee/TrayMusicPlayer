cmake_minimum_required(VERSION 3.20)
project(ThirdPartyInstaller)

include(ExternalProject)

# 安装目录（你希望的 taglib_lib）
set(TAGLIB_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/taglib_lib)
set(TAGLIB_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/taglib)
set(TAGLIB_BUILD_DIR   ${CMAKE_CURRENT_BINARY_DIR}/taglib_build)

# ExternalProject：构建并 install taglib
ExternalProject_Add(
        TagLibProject
        SOURCE_DIR      ${TAGLIB_SOURCE_DIR}
        BINARY_DIR      ${TAGLIB_BUILD_DIR}
        INSTALL_DIR     ${TAGLIB_INSTALL_DIR}
        CMAKE_GENERATOR "MinGW Makefiles"
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX=${TAGLIB_INSTALL_DIR}
        -DBUILD_SHARED_LIBS=ON
        -DVISIBILITY_HIDDEN=ON
        -DWITH_ZLIB=OFF
        -DBUILD_BINDINGS=ON
        -DBUILD_EXAMPLES=OFF
        -DBUILD_TESTING=OFF
        BUILD_COMMAND   ${CMAKE_COMMAND} --build . --config Release
        INSTALL_COMMAND ${CMAKE_COMMAND} --install . --config Release --prefix ${TAGLIB_INSTALL_DIR} --strip
)

add_subdirectory(catch2)
