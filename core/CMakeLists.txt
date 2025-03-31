cmake_minimum_required(VERSION 3.20)
# 搜索所有源文件（排除 interface 目录）
file(GLOB_RECURSE CORE_SOURCES
        include/engine/*.h
        include/backends/*.h
        src/*.cpp
)

add_library(tray_music_core STATIC ${CORE_SOURCES})

# 资源文件
qt_add_resources(tray_music_core "logo"
        FILES logo.qrc
)

# 让 interface 头文件对外可见
target_include_directories(tray_music_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/interface
)

# 其他私有 include 目录
target_include_directories(tray_music_core PRIVATE
        include/backends
        include/engine
)

# 链接 Qt 依赖
target_link_libraries(tray_music_core PRIVATE
        Qt6::Core
        Qt6::Multimedia
)
