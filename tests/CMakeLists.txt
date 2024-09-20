add_executable(directory_test directory_test.c)
target_include_directories(directory_test PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(directory_test cfs)
add_test(NAME directory_test COMMAND directory_test)
