add_executable(test_logger 
        TestLogger.cpp)
add_test(NAME "test_logger" COMMAND $<TARGET_FILE:test_logger>)
target_link_libraries(test_logger PRIVATE catch2)





qt_add_executable(test_ui
        MANUAL_FINALIZATION
        main.cpp
)
target_link_libraries(test_ui PUBLIC
        tray_music_ui
)
target_link_libraries(test_logger PRIVATE tray_music_core)
target_compile_features(test_logger PRIVATE cxx_std_17)

if(WIN32)
    set_target_properties(test_ui PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()
qt_finalize_target(test_ui)
