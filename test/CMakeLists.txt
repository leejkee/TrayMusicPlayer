add_executable(test_logger TestLogger.cpp)
add_executable(test_taglib testTaglib.cpp)

add_test(NAME "test_logger" COMMAND $<TARGET_FILE:test_logger>)
add_test(NAME "test_taglib" COMMAND $<TARGET_FILE:test_taglib>)

qt_add_executable(test_db
        TestDB.cpp
)

target_link_libraries(test_db PRIVATE tray_music_core Qt6::Test Qt6::Core)

target_link_libraries(test_logger PRIVATE tray_music_core catch2)

target_link_libraries(test_taglib PRIVATE Qt6::Core catch2)

