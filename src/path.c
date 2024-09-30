#include "path.h"
#include "util.h"
#include "path_internal.c"

struct cfs_result_bool_t cfs_path_exists(const char* const path)
{
    struct cfs_result_bool_t result;

    struct stat s = {0};
    int stat_result = stat(path, &s);
    if (stat_result == -1)
    {
        if (errno == ENOTDIR || errno == ENOENT)
        {
            cfs_result_set_success(&result.info);
            result.value = false;
            return result;
        }

        cfs_result_set_err_stdlib(&result.info);
        return result;
    }

    cfs_result_set_success(&result.info);
    result.value = true;
    return result;
}

struct cfs_result_string_t cfs_path_home_d()
{
    struct cfs_result_string_t result;
    CFS_IMPL_WIN_OTHER(
        { __dir_get_home_win32_d(&result); },
        { __dir_get_home_other_d(&result); }
    );

    return result;
}

struct cfs_result_size_t cfs_path_home_s(char* buffer, size_t max_buffer_size)
{
    struct cfs_result_size_t result;
    CFS_IMPL_WIN_OTHER(
    {
        __dir_get_home_win32_s(&result, buffer, max_buffer_size);
    },
    {
        __dir_get_home_other_s(&result, buffer, max_buffer_size);
    });

    return result;
}

struct cfs_result_string_t vcfs_path_dir_d(bool use_leading_delim, size_t n, ...)
{
    struct cfs_result_string_t result;

    if (n == 0)
    {
        cfs_result_set_err_invalid_arg(&result.info);
        cfs_result_message_write(&result.info, "Argument 'n' should be > 0.\n");
        return result;
    }
    
    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_d(&result, n, use_leading_delim, component_list);
    va_end(component_list);

    return result;
}

struct cfs_result_size_t vcfs_path_dir_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, ...)
{
    struct cfs_result_size_t result;

    if (n == 0)
    {
        cfs_result_set_err_invalid_arg(&result.info);
        cfs_result_message_write(&result.info, "Argument 'n' should be > 0.\n");
        return result;
    }

    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_s(&result, buffer, max_buffer_size, n, use_leading_delim, component_list);
    va_end(component_list);

    return result;
}

struct cfs_result_string_t cfs_path_dir_d(size_t n, bool use_leading_delim, const char** const components)
{
    struct cfs_result_string_t result;

    if (n == 0 || components == NULL)
    {
        cfs_result_set_err_invalid_arg(&result.info);
        cfs_result_message_write(&result.info, "n must be > 0 and components can't be NULL.\n");
        return result;
    }

    __create_full_path_d(&result, n, use_leading_delim, components);
    return result;
}

struct cfs_result_size_t cfs_path_dir_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, const char** components)
{
    struct cfs_result_size_t result;

    if (n == 0 || components == NULL)
    {
        cfs_result_set_err_invalid_arg(&result.info);
        cfs_result_message_write(&result.info, "n must be > 0 and components can't be NULL.\n");
        return result;
    }

    __create_full_path_s(&result, buffer, max_buffer_size, n, use_leading_delim, components);
    return result;
}

/*

char* vcfs_path_file(size_t n, bool use_leading_delim, ...)
{
    if (n == 0)
        return NULL;

    va_list component_list;
    va_start(component_list, use_leading_delim);
    char* path = __vcreate_full_path(n, use_leading_delim, component_list);
    va_end(component_list);

    if (!path)
        return NULL;

    // remove trailing '/' or '\\' for files
    path[strlen(path) - 1] = '\0';

    return path;
}

char* cfs_path_file(size_t n, bool use_leading_delim, const char** const components)
{
    if (n == 0 || components == NULL)
        return NULL;

    char* path = __create_full_path(n, use_leading_delim, components);
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

char* cfs_path_current_dir()
{
    CFS_IMPL_WIN_OTHER(
    {
        DWORD buff_size = GetCurrentDirectory(0, NULL);
        char* buffer = calloc(buff_size + 1, sizeof(char));
        if (GetCurrentDirectory(buff_size, buffer) == 0)
        {
            free(buffer);
            return NULL;
        }
        return buffer;
    },
    {
        return getcwd(NULL, 0);
    });
}
*/
