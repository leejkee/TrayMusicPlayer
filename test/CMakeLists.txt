cmake_minimum_required(VERSION 3.20)

qt_add_executable(test_ui
        MANUAL_FINALIZATION
        main.cpp
)

target_link_libraries(test_ui PUBLIC
        tray_music_ui
)

if(WIN32)
    set_target_properties(test_ui PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

qt_finalize_target(test_ui)
