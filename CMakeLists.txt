cmake_minimum_required (VERSION 3.5)
project (cxxpy)

set(CMAKE_CXX_STANDARD 17)
include_directories(AFTER include)

add_subdirectory(example)

enable_testing()
add_subdirectory(test)
