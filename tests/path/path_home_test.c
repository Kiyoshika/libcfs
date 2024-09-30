#include "path.h"
#include "result.h"
#include "util.h"

#include <assert.h>

int main()
{
    struct cfs_result_string_t result = cfs_path_home_d();
    assert(result.info.is_error == false);
    assert(result.value != NULL);
    cfs_result_string_free(&result);

    char home[256] = {0};
    struct cfs_result_size_t result2 = cfs_path_home_s(home, 256);
    assert(result2.info.is_error == false);
    assert(strlen(home) > 0);

    return 0;
}
