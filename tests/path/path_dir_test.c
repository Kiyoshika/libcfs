#include "path.h"
#include "result.h"
#include "util.h"

#include <assert.h>

int main()
{
    struct cfs_result_string_t vresult_d = vcfs_path_dir_d(false, 0);
    assert(vresult_d.info.is_error == true);

    // empty path component is a failure
    vresult_d = vcfs_path_dir_d(false, 3, "a", "b", "");
    assert(vresult_d.info.is_error == true);

    vresult_d = vcfs_path_dir_d(false, 3, "some", "random", "path");
    assert(vresult_d.info.is_error == false);
    CFS_IMPL_WIN_OTHER(
    {
        assert(strcmp(vresult_d.value, "some\\random\\path\\") == 0);
    },
    {
        assert(strcmp(vresult_d.value, "some/random/path/") == 0);
    });
    cfs_result_string_free(&vresult_d);

    vresult_d = vcfs_path_dir_d(true, 3, "some", "random", "path");
    assert(vresult_d.info.is_error == false);
    CFS_IMPL_WIN_OTHER(
    {
        assert(strcmp(vresult_d.value, "\\some\\random\\path\\") == 0);
    },
    {
        assert(strcmp(vresult_d.value, "/some/random/path/") == 0);
    });
    cfs_result_string_free(&vresult_d);

    struct cfs_result_size_t vresult_s = vcfs_path_dir_s(false, NULL, 0, 0);
    assert(vresult_s.info.is_error == true);

    char path[256] = {0};
    vresult_s = vcfs_path_dir_s(false, path, 0, 3, "a", "b", "c");
    assert(vresult_s.info.is_error == true);

    vresult_s = vcfs_path_dir_s(false, path, 256, 3, "some", "random", "path");
    assert(vresult_s.info.is_error == false);
    CFS_IMPL_WIN_OTHER(
    {
        assert(strcmp(path, "some\\random\\path\\") == 0);
    },
    {
        assert(strcmp(path, "some/random/path/") == 0);
    });

    vresult_s = vcfs_path_dir_s(false, path, 256, 3, "a", "b", "c");
    assert(vresult_s.info.is_error == false);
    fprintf(stderr, "%s\n", path);
    CFS_IMPL_WIN_OTHER(
    {
        assert(strcmp(path, "a\\b\\c\\") == 0);
    },
    {
        assert(strcmp(path, "a/b/c/") == 0);
    });

    vresult_s = vcfs_path_dir_s(false, path, 256, 3, "a", "b", "");
    assert(vresult_s.info.is_error == true);

    vresult_s = vcfs_path_dir_s(true, path, 256, 3, "some", "random", "path");
    assert(vresult_s.info.is_error == false);
    CFS_IMPL_WIN_OTHER(
    {
        assert(strcmp(path, "\\some\\random\\path\\") == 0);
    },
    {
        assert(strcmp(path, "/some/random/path/") == 0);
    });

    return 0;
}
