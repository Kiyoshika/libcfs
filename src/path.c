#include "path.h"
#include "util.h"
#include "path_internal.c"

struct cfs_result_t cfs_path_exists(const char* const path)
{
    struct cfs_result_t result;

    struct stat s = {0};
    int stat_result = stat(path, &s);
    if (stat_result == -1)
    {
        if (errno == ENOTDIR || errno == ENOENT)
        {
            cfs_result_set_error(&result, false);
            cfs_result_message_writef(&result, "Path '%s' doesn't exist.\n", path);
            return result;
        }

        cfs_result_set_error(&result, true);
        cfs_result_message_write_perror(&result);
        return result;
    }

    cfs_result_set_error(&result, false);
    cfs_result_value_set_bool(&result, true);
    return result;
}

char* cfs_path_home()
{
    CFS_IMPL_WIN_OTHER(
        { return __dir_get_home_win32(); },
        { return __dir_get_home_other(); }
    );
}

char* vcfs_path_dir(size_t n, ...)
{
    if (n == 0)
        return NULL;
    
    va_list component_list;
    va_start(component_list, n);
    char* path = __vcreate_full_path(n, component_list);
    va_end(component_list);

    return path;
}

char* cfs_path_dir(size_t n, const char** const components)
{
    if (n == 0 || components == NULL)
        return NULL;

    return __create_full_path(n, components);
}

char* vcfs_path_file(size_t n, ...)
{
    if (n == 0)
        return NULL;

    va_list component_list;
    va_start(component_list, n);
    char* path = __vcreate_full_path(n, component_list);
    va_end(component_list);

    if (!path)
        return NULL;

    // remove trailing '/' or '\\' for files
    path[strlen(path) - 1] = '\0';

    return path;
}

char* cfs_path_file(size_t n, const char** const components)
{
    if (n == 0 || components == NULL)
        return NULL;

    char* path = __create_full_path(n, components);
    if (!path)
        return NULL;

    // remove tailing '/' or '\\' for files
    path[strlen(path) - 1] = '\0';

    return path;
}

char* vcfs_path_dir_from_home(size_t n, ...)
{
    if (n == 0)
        return NULL;

    va_list component_list;
    va_start(component_list, n);
    char* path = __vcreate_full_path_from_home(n, component_list);
    va_end(component_list);

    return path;
}

char* cfs_path_dir_from_home(size_t n, const char** const components)
{
    if (n == 0 || components == NULL)
        return NULL;

    return __create_full_path_from_home(n, components);
}

char* vcfs_path_file_from_home(size_t n, ...)
{
    if (n == 0)
        return NULL;

    va_list component_list;
    va_start(component_list, n);
    char* path = __vcreate_full_path_from_home(n, component_list);
    va_end(component_list);

    if (!path)
        return NULL;

    path[strlen(path) - 1] = '\0';

    return path;
}

char* cfs_path_file_from_home(size_t n, const char** const components)
{
    if (n == 0 || components == NULL)
        return NULL;

    char* path = __create_full_path_from_home(n, components);
    if (!path)
        return NULL;

    // remove trailing '/' or '\' for files
    path[strlen(path) - 1] = '\0';

    return path;
}
