
cmake_minimum_required (VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
include_directories(../src)

aux_source_directory(. TEST_ENTRIES)
add_executable (cmodel_test ${TEST_ENTRIES})
