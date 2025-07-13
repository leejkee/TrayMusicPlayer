add_library(tray_log STATIC logger/QLogger.cpp)
target_include_directories(tray_log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/logger)
target_link_libraries(tray_log PUBLIC
        Qt6::Core
)

add_library(Tray::tray_log ALIAS tray_log)

add_library(lyric_parser STATIC
        lyricparser/lyricparser.h
        lyricparser/lyricparser.cpp
        lyricparser/scopedfile.h
        lyricparser/scopedfile.cpp
        lyricparser/textfilehelper.cpp
        lyricparser/textfilehelper.h
)
target_include_directories(lyric_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lyricparser)