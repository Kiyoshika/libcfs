#include "path.h"
#include "util.h"
#include "path_internal.c"

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
