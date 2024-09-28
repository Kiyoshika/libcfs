#include "directory.h"
#include "path.h"

#include <assert.h>
#include <stdio.h>

int main()
{
    // if a path doesn't exist, it will be an error
    char* fake_dir_path = vcfs_path_dir(3, "some", "fake", "path");
    struct cfs_result_t result = cfs_dir_exists(fake_dir_path);
    assert(result.is_error == true);
    free(fake_dir_path);

    // home directory should be a valid path
    char* home_path = cfs_path_home();
    result = cfs_dir_exists(home_path);
    assert(result.is_error == false);
    assert(cfs_result_value_get_bool(&result) == true);
    free(home_path);

    // existing path to a directory
    char* current_dir_path = cfs_path_current_dir();
    char* path = vcfs_path_dir(2, current_dir_path, "sample-dir");
    result = cfs_dir_exists(path);
    fprintf(stderr, "%s\n", result.msg);
    assert(result.is_error == false);
    assert(cfs_result_value_get_bool(&result) == true);

    // existing path but to a file, not a directory
    free(path);
    path = vcfs_path_file(3, current_dir_path, "sample-dir", "sample-file.txt");
    result = cfs_dir_exists(path);
    assert(result.is_error == false);
    assert(cfs_result_value_get_bool(&result) == false);

    free(path);
    free(current_dir_path);
    return 0;
}
