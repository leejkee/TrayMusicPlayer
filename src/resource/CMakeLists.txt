cmake_minimum_required(VERSION 3.20)

add_library(tray_qrc STATIC
        qss.qrc
        svg.qrc
)

target_include_directories(tray_qrc PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_property(TARGET tray_qrc PROPERTY AUTORCC ON)

target_link_libraries(tray_qrc PRIVATE
        Qt6::Core
)
