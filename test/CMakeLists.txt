cmake_minimum_required(VERSION 3.20)

qt_add_executable(Test
        MANUAL_FINALIZATION
        main.cpp
)

target_link_libraries(Test PUBLIC
        tray_music_ui
)

if(WIN32)
    set_target_properties(Test PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

qt_finalize_target(Test)
