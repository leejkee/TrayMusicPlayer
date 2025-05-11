include(ExternalProject)

# 设置安装目录为项目内的 3rdparty/taglib/taglib_lib
set(TAGLIB_INSTALL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/taglib_lib)
file(MAKE_DIRECTORY ${TAGLIB_INSTALL_DIR})  # 确保目录存在

ExternalProject_Add(
        TagLib_lib
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdParty/taglib
        BINARY_DIR ${CMAKE_BINARY_DIR}/3rdParty/taglib_build
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${TAGLIB_INSTALL_DIR}
        -DBUILD_SHARED_LIBS=ON
        -DBUILD_BINDINGS=OFF
        -DBUILD_EXAMPLES=OFF
        -DBUILD_TESTING=OFF
        -DVISIBILITY_HIDDEN=ON
        -DWITH_ZLIB=OFF
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_CXX_STANDARD=17
        -G "Ninja"
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        BUILD_COMMAND ${CMAKE_COMMAND} --build . --config Release
        INSTALL_COMMAND ${CMAKE_COMMAND} --install . --config Release --strip
        UPDATE_COMMAND ""
)
