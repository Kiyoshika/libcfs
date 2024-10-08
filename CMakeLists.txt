cmake_minimum_required(VERSION 3.11)

project(cfs C)

set(CMAKE_C_STANDARD 99)

option(DEBUG_MODE OFF)
option(COMPILE_TESTS OFF)

if (DEBUG_MODE)
    message("COMPILING IN DEBUG MODE")
    set(CMAKE_C_FLAGS "-O0 -g -Wall -Wextra -fsanitize=address")
else()
    message("COMPILING IN RELEASE MODE")
    set(CMAKE_C_FLAGS "-O2 -Wall -Wextra")
endif()

add_subdirectory(src)

if (COMPILE_TESTS)
    message("COMPILING TESTS")
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/sample-dir/sample-file.txt ${CMAKE_CURRENT_BINARY_DIR}/tests/sample-dir/sample-file.txt COPYONLY)
endif()
