
qt_add_executable(test_db
        TestDB.cpp
)

target_link_libraries(test_db PRIVATE tray_music_core Qt6::Test Qt6::Core)


