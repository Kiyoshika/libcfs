add_executable(path_exists_test path_exists_test.c)
target_include_directories(path_exists_test PUBLIC ${CMAKE_CURRENT_PROJECT_DIR}/include)
target_link_libraries(path_exists_test cfs)
add_test(NAME path_exists_test COMMAND path_exists_test)

add_executable(path_home_test path_home_test.c)
target_include_directories(path_home_test PUBLIC ${CMAKE_CURRENT_PROJECT_DIR}/include)
target_link_libraries(path_home_test cfs)
add_test(NAME path_home_test COMMAND path_home_test)

add_executable(path_dir_test path_dir_test.c)
target_include_directories(path_dir_test PUBLIC ${CMAKE_CURRENT_PROJECT_DIR}/include)
target_link_libraries(path_dir_test cfs)
add_test(NAME path_dir_test COMMAND path_dir_test)

add_executable(path_file_test path_file_test.c)
target_include_directories(path_file_test PUBLIC ${CMAKE_CURRENT_PROJECT_DIR}/include)
target_link_libraries(path_file_test cfs)
add_test(NAME path_file_test COMMAND path_file_test)
