message(STATUS "Test Module ON")

## TestConfigWidget
qt_add_executable(test_config_widget TestConfigWidget.cpp)
target_link_libraries(test_config_widget PRIVATE tray_music_ui)
## TestConfigWidget

## TestSplitter
qt_add_executable(test_splitter_ui TestSplitter.cpp)
target_link_libraries(test_splitter_ui PRIVATE tray_music_ui)
## TestSplitter

## TestLyricParser
add_executable(TestLyricParser TestLyricParser.cpp)
target_link_libraries(TestLyricParser PRIVATE catch2 lyric_parser)
add_test(NAME TestLyricParser COMMAND TestLyricParser)
## TestLyricParser

