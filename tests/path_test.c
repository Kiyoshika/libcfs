#include "path.h"
#include "util.h"

#include <assert.h>
#include <stdio.h>

int main()
{
    // no arguments will always return NULL

    char* path = vcfs_path_dir(0);
    assert(path == NULL);

    path = cfs_path_dir(0, NULL);
    assert(path == NULL);

    path = vcfs_path_file(0);
    assert(path == NULL);

    path = cfs_path_file(0, NULL);
    assert(path == NULL);

    path = vcfs_path_dir_from_home(0);
    assert(path == NULL);

    path = cfs_path_dir_from_home(0, NULL);
    assert(path == NULL);

    path = vcfs_path_file_from_home(0);
    assert(path == NULL);

    path = cfs_path_file_from_home(0, NULL);
    assert(path == NULL);

    // some edge cases
    // (any length 0 component will return NULL)

    char** dynamic_path1 = calloc(3, sizeof(char*));
    dynamic_path1[0] = strdup("");
    dynamic_path1[1] = strdup("");
    dynamic_path1[2] = strdup("");

    char** dynamic_path2 = calloc(3, sizeof(char*));
    dynamic_path2[0] = strdup("a");
    dynamic_path2[1] = strdup("b");
    dynamic_path2[2] = strdup("");

    path = vcfs_path_dir(3, "", "", "");
    assert(path == NULL);

    path = cfs_path_dir(3, (const char** const)dynamic_path1);
    assert(path == NULL);

    path = vcfs_path_dir(3, "a", "b", "");
    assert(path == NULL);

    path = cfs_path_dir(3, (const char** const)dynamic_path2);
    assert(path == NULL);

    path = vcfs_path_file(3, "", "", "");
    assert(path == NULL);

    path = cfs_path_file(3, (const char** const)dynamic_path1);
    assert(path == NULL);

    path = vcfs_path_file(3, "a", "b", "");
    assert(path == NULL);

    path = cfs_path_file(3, (const char** const)dynamic_path2);
    assert(path == NULL);

    path = vcfs_path_dir_from_home(3, "", "", "");
    assert(path == NULL);

    path = cfs_path_dir_from_home(3, (const char** const)dynamic_path1);
    assert(path == NULL);

    path = vcfs_path_dir_from_home(3, "a", "b", "");
    assert(path == NULL);

    path = cfs_path_dir_from_home(3, (const char** const)dynamic_path2);
    assert(path == NULL);

    path = vcfs_path_file_from_home(3, "", "", "");
    assert(path == NULL);

    path = cfs_path_file_from_home(3, (const char** const)dynamic_path1);
    assert(path == NULL);

    path = vcfs_path_file_from_home(3, "a", "b", "");
    assert(path == NULL);

    path = cfs_path_file_from_home(3, (const char** const)dynamic_path2);
    assert(path == NULL);

    free(dynamic_path1[0]);
    free(dynamic_path1[1]);
    free(dynamic_path1[2]);
    free(dynamic_path1);

    free(dynamic_path2[0]);
    free(dynamic_path2[1]);
    free(dynamic_path2[2]);
    free(dynamic_path2);

    // normal cases

    path = vcfs_path_dir(3, "some", "random", "path");
    CFS_IMPL_WIN_OTHER(
    {
        assert(strcmp(path, "\\some\\random\\path\\") == 0);
    },
    {
        assert(strcmp(path, "/some/random/path/") == 0);
    });
    free(path);

    // this could be a dynamically generated path at runtime in your program
    char** components = calloc(3, sizeof(char*));
    components[0] = strdup("some");
    components[1] = strdup("random");
    components[2] = strdup("path");

    path = cfs_path_dir(3, (const char** const)components);
    CFS_IMPL_WIN_OTHER(
    {
        assert(strcmp(path, "\\some\\random\\path\\") == 0);
    },
    {
        assert(strcmp(path, "/some/random/path/") == 0);
    });
    free(path);
    free(components[0]);
    free(components[1]);
    free(components[2]);
    free(components);

    path = vcfs_path_file(4, "some", "random", "path", "file.txt");
    CFS_IMPL_WIN_OTHER(
    {
        assert(strcmp(path, "\\some\\random\\path\\file.txt") == 0);
    },
    {
        assert(strcmp(path, "/some/random/path/file.txt") == 0);
    });

    free(path);

    return 0;
}
