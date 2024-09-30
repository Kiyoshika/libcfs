#include "path.h"
#include "util.h"
#include "path_internal.c"

#define CHECK_ARGS_BUFFER(_result, _buffer, _max_buffer_size){\
    if (!_buffer || _max_buffer_size == 0)\
    {\
        cfs_result_set_err_invalid_arg(&_result.info);\
        cfs_result_message_write(&_result.info, "buffer cannot be NULL and max_buffer_size must be > 0.\n");\
        return _result;\
    }\
}

#define CHECK_ARGS_N(_result, _n){\
    if (_n == 0)\
    {\
        cfs_result_set_err_invalid_arg(&_result.info);\
        cfs_result_message_write(&_result.info, "n must be > 0.\n");\
        return _result;\
    }\
}

#define CHECK_ARGS_COMPONENTS(_result, _components){\
    if (_components == NULL)\
    {\
        cfs_result_set_err_invalid_arg(&_result.info);\
        cfs_result_message_write(&_result.info, "components cannot be NULL.\n");\
        return _result;\
    }\
}

struct cfs_result_bool_t cfs_path_exists(const char* const path)
{
    struct cfs_result_bool_t result;

    if (!path)
    {
        cfs_result_set_err_invalid_read(&result.info);
        cfs_result_message_write(&result.info, "Path cannot be NULL.\n");
        return result;
    }

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

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);

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

    CHECK_ARGS_N(result, n);
    
    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_d(&result, n, use_leading_delim, component_list);
    va_end(component_list);

    return result;
}

struct cfs_result_size_t vcfs_path_dir_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, ...)
{
    struct cfs_result_size_t result;

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);
    CHECK_ARGS_N(result, n);

    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_s(&result, buffer, max_buffer_size, n, use_leading_delim, component_list);
    va_end(component_list);

    return result;
}

struct cfs_result_string_t cfs_path_dir_d(size_t n, bool use_leading_delim, const char** const components)
{
    struct cfs_result_string_t result;

    CHECK_ARGS_N(result, n);
    CHECK_ARGS_COMPONENTS(result, components);

    __create_full_path_d(&result, n, use_leading_delim, components);
    return result;
}

struct cfs_result_size_t cfs_path_dir_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, const char** components)
{
    struct cfs_result_size_t result;

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);
    CHECK_ARGS_N(result, n);
    CHECK_ARGS_COMPONENTS(result, components);

    __create_full_path_s(&result, buffer, max_buffer_size, n, use_leading_delim, components);
    return result;
}

struct cfs_result_string_t vcfs_path_file_d(bool use_leading_delim, size_t n, ...)
{
    struct cfs_result_string_t result;

    CHECK_ARGS_N(result, n);

    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_d(&result, n, use_leading_delim, component_list);
    va_end(component_list);

    if (result.info.is_error)
        return result;

    // remove trailing '/' or '\\' for files
    char* path = result.value;
    size_t path_len = strlen(path);
    if (path_len > 0)
        path[path_len - 1] = '\0';

    return result;
}

struct cfs_result_size_t vcfs_path_file_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, ...)
{
    struct cfs_result_size_t result;

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);

    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_s(&result, buffer, max_buffer_size, n, use_leading_delim, component_list);
    va_end(component_list);

    if (result.info.is_error)
        return result;

    size_t buffer_len = strlen(buffer);
    if (buffer_len > 0)
        buffer[buffer_len - 1] = '\0';

    return result;
}

struct cfs_result_string_t cfs_path_file_d(bool use_leading_delim, size_t n, const char** const components)
{
    struct cfs_result_string_t result;

    CHECK_ARGS_N(result, n);
    CHECK_ARGS_COMPONENTS(result, components);

    __create_full_path_d(&result, n, use_leading_delim, components);
    if (result.info.is_error)
        return result;

    char* path = result.value;
    size_t path_len = strlen(path);

    if (path_len > 0)
        path[path_len - 1] = '\0';

    return result;
}

struct cfs_result_size_t cfs_path_file_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, const char** components)
{
    struct cfs_result_size_t result;

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);
    CHECK_ARGS_N(result, n);
    CHECK_ARGS_COMPONENTS(result, components);

    __create_full_path_s(&result, buffer, max_buffer_size, use_leading_delim, n, components);
    if (result.info.is_error)
        return result;

    size_t buffer_len = strlen(buffer);
    if (buffer_len > 0)
        buffer[buffer_len - 1] = '\0';

    return result;
}

struct cfs_result_string_t vcfs_path_dir_from_home_d(size_t n, ...)
{
    struct cfs_result_string_t result;

    CHECK_ARGS_N(result, n);

    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_from_home_d(&result, n, component_list);
    va_end(component_list);

    return result;
}

struct cfs_result_size_t vcfs_path_dir_from_home_s(char* buffer, size_t max_buffer_size, size_t n, ...)
{
    struct cfs_result_size_t result;

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);
    CHECK_ARGS_N(result, n);

    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_from_home_s(&result, buffer, max_buffer_size, n, component_list);
    va_end(component_list);

    return result;
}

struct cfs_result_string_t cfs_path_dir_from_home_d(size_t n, const char** components)
{
    struct cfs_result_string_t result;

    CHECK_ARGS_N(result, n);
    CHECK_ARGS_COMPONENTS(result, components);

    __create_full_path_from_home_d(&result, n, components);

    return result;
}

struct cfs_result_size_t cfs_path_dir_from_home_s(char* buffer, size_t max_buffer_size, size_t n, const char** components)
{
    struct cfs_result_size_t result;

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);
    CHECK_ARGS_N(result, n);

    __create_full_path_from_home_s(&result, buffer, max_buffer_size, n, components);

    return result;
}

struct cfs_result_string_t vcfs_path_file_from_home_d(size_t n, ...)
{
    struct cfs_result_string_t result;

    CHECK_ARGS_N(result, n);

    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_from_home_d(&result, n, component_list);
    va_end(component_list);

    if (result.info.is_error)
        return result;

    char* path = result.value;
    size_t path_len = strlen(result.value);

    if (path_len > 0)
        path[path_len - 1] = '\0';

    return result;
}

struct cfs_result_size_t vcfs_path_file_from_home_s(char* buffer, size_t max_buffer_size, size_t n, ...)
{
    struct cfs_result_size_t result;

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);
    CHECK_ARGS_N(result, n);

    va_list component_list;
    va_start(component_list, n);
    __vcreate_full_path_from_home_s(&result, buffer, max_buffer_size, n, component_list);
    va_end(component_list);

    if (result.info.is_error)
        return result;

    size_t buffer_len = strlen(buffer);
    if (buffer_len > 0)
        buffer[buffer_len - 1] = '\0';

    return result;
}

struct cfs_result_string_t cfs_path_file_from_home_d(size_t n, const char** components)
{
    struct cfs_result_string_t result;
    
    CHECK_ARGS_N(result, n);
    CHECK_ARGS_COMPONENTS(result, components);

    __create_full_path_from_home_d(&result, n, components);

    if (result.info.is_error)
        return result;

    char* path = result.value;
    size_t path_len = strlen(path);

    if (path_len > 0)
        path[path_len - 1] = '\0';

    return result;
}

struct cfs_result_size_t cfs_path_file_from_home_s(char* buffer, size_t max_buffer_size, size_t n, const char** components)
{
    struct cfs_result_size_t result;

    CHECK_ARGS_BUFFER(result, buffer, max_buffer_size);
    CHECK_ARGS_N(result, n);

    __create_full_path_from_home_s(&result, buffer, max_buffer_size, n, components);

    size_t buffer_len = strlen(buffer);
    if (buffer_len > 0)
        buffer[buffer_len - 1] = '\0';

    return result;
}

struct cfs_result_string_t cfs_path_current_dir_d()
{
    struct cfs_result_string_t result;

    CFS_IMPL_WIN_OTHER(
    {
        __current_dir_win32_d(&result);
    },
    {
        __current_dir_other_d(&result);
    });

    return result;
}

struct cfs_result_size_t cfs_path_current_dir_s(char* buffer, size_t max_buffer_size)
{
    struct cfs_result_size_t result;

    CFS_IMPL_WIN_OTHER(
    {
        __current_dir_win32_s(&result, buffer, max_buffer_size);
    },
    {
        __current_dir_other_s(&result, buffer, max_buffer_size);
    });

    return result;
}
