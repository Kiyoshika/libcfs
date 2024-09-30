#include "directory.h"
#include "util.h"
#include "path.h"
#include <sys/stat.h>

/*
struct cfs_result_t cfs_dir_exists(const char* const path)
{
    struct cfs_result_t result;

    result = cfs_path_exists(path);
    if (result.is_error)
        return result;

    if (!cfs_result_value_get_bool(&result))
    {
        cfs_result_set_error(&result, true);
        cfs_result_message_writef(&result, "Path '%s' doesn't exist.\n", path);
        return result;
    }

    cfs_result_set_error(&result, false);

    struct stat s = {0};
    stat(path, &s);

    if (S_ISDIR(s.st_mode))
    {
        cfs_result_value_set_bool(&result, true);
        return result;
    }

    cfs_result_value_set_bool(&result, false);
    return result;
}
*/
