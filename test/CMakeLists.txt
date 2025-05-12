qt_add_executable(test_db
        TestDB.cpp
)

target_link_libraries(test_db PRIVATE Qt6::Test tray_database_for_Test)
