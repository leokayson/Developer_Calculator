# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\dev_calc_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\dev_calc_autogen.dir\\ParseCache.txt"
  "dev_calc_autogen"
  )
endif()
