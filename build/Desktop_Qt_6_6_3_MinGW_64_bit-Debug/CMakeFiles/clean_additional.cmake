# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\cashnote_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\cashnote_autogen.dir\\ParseCache.txt"
  "cashnote_autogen"
  )
endif()
