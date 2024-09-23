add_executable(path_test path_test.c)
target_include_directories(path_test PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(path_test cfs)
add_test(NAME path_test COMMAND path_test)

add_executable(result_test result_test.c)
target_include_directories(result_test PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(result_test cfs)
add_test(NAME result_test COMMAND result_test)
