message(STATUS "Test Module ON")

## TestConfigWidget
qt_add_executable(test_config_widget TestConfigWidget.cpp)
target_link_libraries(test_config_widget PRIVATE tray_music_ui)
## TestConfigWidget

## TestSplitter
qt_add_executable(test_splitter_ui TestSplitter.cpp)
target_link_libraries(test_splitter_ui PRIVATE tray_music_ui)
## TestSplitter

## TestListView
qt_add_executable(test_Lyric_view TestLyricView.cpp)
target_link_libraries(test_Lyric_view PRIVATE tray_music_ui)
## TestListView
