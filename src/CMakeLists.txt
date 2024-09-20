file(GLOB_RECURSE SRC_ROOT ${PROJECT_SOURCE_DIR}/src/*.c)
add_library(cfs ${SRC_ROOT})
target_include_directories(cfs PUBLIC ${PROJECT_SOURCE_DIR}/include)
