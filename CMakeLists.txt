cmake_minimum_required (VERSION 3.5)
project (sym CXX)

option(SYM_BUILD_EXAMPLE "enable examples for sym" ON)
option(SYM_BUILD_UNIT_TEST "enable testing for sym" ON)

set(CMAKE_CXX_STANDARD 17)
include_directories(AFTER include)

if (SYM_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

if (SYM_BUILD_UNIT_TEST)
  enable_testing()
  add_subdirectory(test)
endif()
