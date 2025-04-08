add_executable(test_logger TestLogger.cpp)
add_executable(test_taglib testTaglib.cpp)

add_test(NAME "test_logger" COMMAND $<TARGET_FILE:test_logger>)
add_test(NAME "test_taglib" COMMAND $<TARGET_FILE:test_taglib>)

qt_add_executable(test_ui
        MANUAL_FINALIZATION
        main.cpp
)

add_executable(test_string testString.cpp)
target_link_libraries(test_string PRIVATE Qt6::Core)

target_link_libraries(test_ui PUBLIC tray_music_ui)

target_link_libraries(test_logger PRIVATE tray_music_core catch2)

target_link_libraries(test_taglib PRIVATE Qt6::Core catch2)

target_link_options(test_taglib PRIVATE -municode)

target_link_options(test_logger PRIVATE -municode)
if(WIN32)
    set_target_properties(test_ui PROPERTIES
            WIN32_EXECUTABLE OFF
    )
endif()
qt_finalize_target(test_ui)


