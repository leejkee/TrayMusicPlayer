qt_add_executable(test_config_widget TestConfigWidget.cpp)
message("test")
target_link_libraries(test_config_widget PRIVATE tray_music_ui)