# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TrayMusicPlayer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TrayMusicPlayer_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\tray_music_core_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\tray_music_core_autogen.dir\\ParseCache.txt"
  "TrayMusicPlayer_autogen"
  "tray_music_core_autogen"
  )
endif()
