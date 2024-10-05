#include "path.h"
#include "result.h"
#include "util.h"

#include <assert.h>

int main()
{
    struct cfs_result_bool_t result = cfs_path_exists("/some/fake/path");
    assert(result.info.is_error == false);
    assert(result.value == false);

    result = cfs_path_exists(NULL);
    assert(result.info.is_error == true);
    assert(result.info.error_type == ECFS_ERR_INVALID_READ);

    result = cfs_path_exists("");
    assert(result.info.is_error == false);
    assert(result.value == false);

    char path[256] = {0};
    struct cfs_result_size_t path_result = cfs_path_current_dir_s(path, 256);
    assert(path_result.info.is_error == false);
    result = cfs_path_exists(path);
    assert(result.info.is_error == false);
    assert(result.value == true);

    return 0;
}
