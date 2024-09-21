#include "path.h"
#include "util.h"

#include <assert.h>
#include <stdio.h>

int main()
{
    // no arguments will always return NULL

    char* path = vcfs_path_dir(0);
    assert(path == NULL);

    path = vcfs_path_file(0);
    assert(path == NULL);

    // some edge cases
    // (any length 0 component will return NULL)

    path = vcfs_path_dir(3, "", "", "");
    assert(path == NULL);

    path = vcfs_path_dir(3, "a", "b", "");
    assert(path == NULL);

    path = vcfs_path_file(3, "", "", "");
    assert(path == NULL);

    path = vcfs_path_file(3, "a", "b", "");
    assert(path == NULL);

    path = vcfs_path_dir_from_home(3, "", "", "");
    assert(path == NULL);

    path = vcfs_path_dir_from_home(3, "a", "b", "");
    assert(path == NULL);

    path = vcfs_path_file_from_home(3, "", "", "");
    assert(path == NULL);

    path = vcfs_path_file_from_home(3, "a", "b", "");
    assert(path == NULL);

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
