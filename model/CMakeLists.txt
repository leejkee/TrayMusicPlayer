add_library(tray_lyric_model STATIC
        lyricmodel/lyricmodel.h
        lyricmodel.cpp
)
target_link_libraries(tray_lyric_model PRIVATE Qt6::Core)
target_include_directories(tray_lyric_model PUBLIC lyricmodel)