// internal functions to be included in the main implementation file

static size_t __vget_total_component_length(size_t n, va_list component_list)
{
    size_t total_len = 0;
    for (size_t i = 0; i < n; ++i)
    {
        const char* component = va_arg(component_list, const char*);
        total_len += strlen(component);
    }

    return total_len;
}

static size_t __get_total_component_length(size_t n, const char** components)
{
    size_t total_len = 0;
    for (size_t i = 0; i < n; ++i)
    {
        const char* component = components[i];
        total_len += strlen(component);
    }

    return total_len;
}

static char* __valloc_path_buffer(size_t n, va_list path_component)
{
    size_t total_component_len = __vget_total_component_length(n, path_component);
    if (total_component_len == 0)
        return NULL;

    // starting and ending path delim + each path component (n) + null term
    size_t extra_buffer = 3 + n; 

    return calloc(total_component_len + extra_buffer, sizeof(char));
}

static char* __alloc_path_buffer(size_t n, const char** components)
{
    size_t total_component_len = __get_total_component_length(n, components);
    if (total_component_len == 0)
        return NULL;

    // starting and ending path delim + each path component (n) + null term
    size_t extra_buffer = 3 + n;

    return calloc(total_component_len + extra_buffer, sizeof(char));
}

static void __append_path_delim(char* path_buffer)
{
    CFS_IMPL_WIN_OTHER(
    {
        strcat(path_buffer, "\\");
    },
    {
        strcat(path_buffer, "/");
    });
}

static void __vcreate_full_path_d(struct cfs_result_string_t* result, size_t n, bool use_leading_delim, va_list component_list)
{
    va_list path_component;
    va_list path_component_copy;
    va_copy(path_component, component_list);
    va_copy(path_component_copy, component_list);

    // use one iteration to allocate the path buffer
    char* path_buffer = __valloc_path_buffer(n, path_component_copy);
    va_end(path_component);

    if (!path_buffer)
    {
        cfs_result_set_err_no_mem(&result->info);
        return;
    }

    if (use_leading_delim)
        __append_path_delim(path_buffer);

    // use another iteration to construct the path
    for (size_t i = 0; i < n; ++i)
    {
        const char* component = va_arg(path_component_copy, const char*);
        if (strlen(component) == 0)
        {
            free(path_buffer);
            cfs_result_set_err_invalid_arg(&result->info);
            cfs_result_message_write(&result->info, "Path component cannot be length 0.\n");
            return;
        }

        strcat(path_buffer, component);
        __append_path_delim(path_buffer);
    }
    va_end(path_component_copy);

    result->value = path_buffer;
    cfs_result_set_success(&result->info);
}

static void __vcreate_full_path_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size, size_t n, bool use_leading_delim, va_list component_list)
{
    va_list path_component;
    va_list path_component_copy;
    va_copy(path_component, component_list);
    va_copy(path_component_copy, path_component);

    size_t required_buffer_size = __vget_total_component_length(n, path_component_copy) + 1;
    va_end(path_component_copy);

    if (use_leading_delim)
        __append_path_delim(buffer);

    max_buffer_size -= 1;

    for (size_t i = 0; i < n; ++i)
    {
        const char* component = va_arg(path_component, const char*);
        size_t component_len = strlen(component);
        if (component_len == 0)
        {
            cfs_result_set_err_invalid_arg(&result->info);
            cfs_result_message_write(&result->info, "Path component can not have length 0.\n");
            return;
        }

        strncat(buffer, component, component_len);
        if (component_len >= max_buffer_size)
        {
            cfs_result_set_err_buffer_limit(&result->info);
            result->value = required_buffer_size;
            return;
        }
        max_buffer_size -= component_len;
        if (max_buffer_size == 0)
        {
            cfs_result_set_err_buffer_limit(&result->info);
            result->value = required_buffer_size;
            return;
        }

        if (i < n - 1)
        {
            __append_path_delim(buffer);
            max_buffer_size -= 1;
        }
    }
    va_end(path_component);

    if (max_buffer_size == 0)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        result->value = required_buffer_size;
        return;
    }

    __append_path_delim(buffer);
    cfs_result_set_success(&result->info);
}

static inline void __dir_get_home_other_d(struct cfs_result_string_t* result)
{
    char* home_ptr = getenv("HOME");
    if (!home_ptr)
    {
        cfs_result_set_err_invalid_read(&result->info);
        cfs_result_message_write(&result->info, "Couldn't find environment variable 'HOME'.\n");
        return;
    }

    result->value = strdup(home_ptr);
    if (!result->value)
    {
        cfs_result_set_err_no_mem(&result->info);
        return;
    }

    cfs_result_set_success(&result->info);
}

static inline void __dir_get_home_other_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size)
{
    char* home_ptr = getenv("HOME");
    if (!home_ptr)
    {
        cfs_result_set_err_invalid_read(&result->info);
        cfs_result_message_write(&result->info, "Couldn't find environment variable 'HOME'.\n");
        return;
    }

    strncat(buffer, home_ptr, max_buffer_size);
    if (strlen(home_ptr) >= max_buffer_size)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        result->value = strlen(home_ptr) + 1;
    }
    else
        cfs_result_set_success(&result->info);
}

static inline void __dir_get_home_win32_d(struct cfs_result_string_t* result)
{
    char* home_drive = getenv("HOMEDRIVE");
    char* home_path = getenv("HOMEPATH");
    if (!home_drive || !home_path)
    {
        cfs_result_set_err_invalid_read(&result->info);
        cfs_result_message_write(&result->info, "Couldn't find environment variable(s) 'HOMEDRIVE' and/or 'HOMEPATH'.\n");
        return;
    }

    size_t home_size = strlen(home_drive) + strlen(home_path) + 1;
    char* home = calloc(home_size, sizeof(char));
    if (!home)
    {
        cfs_result_set_err_no_mem(&result->info);
        return;
    }
        
    strcat(home, home_drive);
    strcat(home, home_path);

    cfs_result_set_success(&result->info);
    result->value = home;
}

static inline void __dir_get_home_win32_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size)
{
    char* home_drive = getenv("HOMEDRIVE");
    char* home_path = getenv("HOMEPATH");
    if (!home_drive || !home_path)
    {
        cfs_result_set_err_invalid_read(&result->info);
        cfs_result_message_write(&result->info, "Couldn't find environment variable(s) 'HOMEDRIVE' and/or 'HOMEPATH'.\n");
        return;
    }

    size_t home_drive_len = strlen(home_drive);
    size_t home_path_len = strlen(home_path);
    size_t required_buffer_len = home_drive_len + home_path_len + 1;

    strncat(buffer, home_drive, max_buffer_size);

    if (home_drive_len >= max_buffer_size)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        result->value = required_buffer_len;
        return;
    }

    max_buffer_size -= home_drive_len;

    strncat(buffer, home_path, max_buffer_size);

    if (home_drive_len >= max_buffer_size)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        result->value = required_buffer_len;
        return;
    }

    cfs_result_set_success(&result->info);
}

static void __create_full_path_d(struct cfs_result_string_t* result, size_t n, bool use_leading_delim, const char** components)
{
    char* path_buffer = __alloc_path_buffer(n, components);
    if (!path_buffer)
    {
        cfs_result_set_err_no_mem(&result->info);
        return;
    }

    if (use_leading_delim)
        __append_path_delim(path_buffer);

    for (size_t i = 0; i < n; ++i)
    {
        const char* component = components[i];
        if (strlen(component) == 0)
        {
            free(path_buffer);
            cfs_result_set_err_invalid_arg(&result->info);
            cfs_result_message_write(&result->info, "Path component cannot be length 0.\n");
            return;
        }
        strcat(path_buffer, component);
        __append_path_delim(path_buffer);
    }

    cfs_result_set_success(&result->info);
    result->value = path_buffer;
}

static void __create_full_path_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size, size_t n, bool use_leading_delim, const char** components)
{
    size_t required_buffer_size = __get_total_component_length(n, components) + 1;

    if (use_leading_delim)
        __append_path_delim(buffer);

    max_buffer_size -= 1;

    for (size_t i = 0; i < n; ++i)
    {
        const char* component = components[i];
        size_t component_len = strlen(component);
        if (component_len == 0)
        {
            cfs_result_set_err_invalid_arg(&result->info);
            cfs_result_message_write(&result->info, "Path component cannot have length 0.\n");
            return;
        }

        strncat(buffer, component, component_len);
        if (component_len >= max_buffer_size)
        {
            cfs_result_set_err_buffer_limit(&result->info);
            result->value = required_buffer_size;
            return;

        }
        max_buffer_size -= component_len;
    }

    if (max_buffer_size == 0)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        result->value = required_buffer_size;
        return;
    }

    __append_path_delim(buffer);
    cfs_result_set_success(&result->info);
}

static void __vcreate_full_path_from_home_d(struct cfs_result_string_t* result, size_t n, va_list component_list)
{
    struct cfs_result_string_t home_result = cfs_path_home_d();
    if (home_result.info.is_error)
        *result = home_result;

    // remove trailing '/' or '\' at end of home (if present)
    // since it will be added back when appending path
    size_t end_home = strlen(home_result.value) - 1;
    if (home_result.value[end_home] == '/' || home_result.value[end_home] == '\\')
        home_result.value[end_home] = '\0';

    struct cfs_result_string_t path_result;
    __vcreate_full_path_d(&path_result, n, false, component_list);
    if (path_result.info.is_error)
    {
        cfs_result_string_free(&home_result);
        *result = path_result;
        return;
    }

    size_t len = strlen(home_result.value) + strlen(path_result.value) + 1;
    char* full_path = calloc(len, sizeof(char));
    if (!full_path)
    {
        cfs_result_string_free(&path_result);
        cfs_result_string_free(&home_result);
        cfs_result_set_err_no_mem(&result->info);
        return;
    }

    strcat(full_path, home_result.value);
    strcat(full_path, path_result.value);

    cfs_result_string_free(&home_result);
    cfs_result_string_free(&path_result);

    cfs_result_set_success(&result->info);
    result->value = full_path;
}

static void __vcreate_full_path_from_home_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size, size_t n, va_list component_list)
{
    va_list path_component;
    va_copy(path_component, component_list);
    size_t required_buffer_size = __vget_total_component_length(n, path_component);
    va_end(path_component);

    struct cfs_result_size_t home_result = cfs_path_home_s(buffer, max_buffer_size);
    if (home_result.info.is_error)
    {
        *result = home_result;
        return;
    }

    size_t buffer_len = strlen(buffer);
    max_buffer_size -= buffer_len;
    if (max_buffer_size == 0)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        return;
    }

    __append_path_delim(buffer);
    max_buffer_size -= 1;

    struct cfs_result_size_t path_result;
    __vcreate_full_path_s(&path_result, buffer, max_buffer_size, n, false, component_list);
    if (path_result.info.is_error)
    {
        *result = path_result;
        return;
    }
    max_buffer_size -= strlen(buffer);

    if (max_buffer_size == 0)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        result->value = required_buffer_size;
        return;
    }

    __append_path_delim(buffer);
    cfs_result_set_success(&result->info);
}

static void __create_full_path_from_home_d(struct cfs_result_string_t* result, size_t n, const char** components)
{
    struct cfs_result_string_t home_result = cfs_path_home_d();
    if (home_result.info.is_error)
    {
        *result = home_result;
        return;
    }

    // remove trailing '/' or '\' at end of home (if present)
    // since it will be added back when appending path
    size_t end_home = strlen(home_result.value) - 1;
    if (home_result.value[end_home] == '/' || home_result.value[end_home] == '\\')
        home_result.value[end_home] = '\0';

    struct cfs_result_string_t path_result;
    __create_full_path_d(&path_result, n, false, components);
    if (path_result.info.is_error)
    {
        cfs_result_string_free(&home_result);
        *result = path_result;
        return;
    }

    size_t len = strlen(home_result.value) + strlen(path_result.value) + 1;
    char* full_path = calloc(len, sizeof(char));
    if (!full_path)
    {
        cfs_result_set_err_no_mem(&result->info);
        cfs_result_string_free(&home_result);
        cfs_result_string_free(&path_result);
        return;
    }

    strcat(full_path, home_result.value);
    strcat(full_path, path_result.value);

    cfs_result_string_free(&home_result);
    cfs_result_string_free(&path_result);

    cfs_result_set_success(&result->info);
    result->value = full_path;
}

static void __create_full_path_from_home_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size, size_t n, const char** components)
{
    size_t required_buffer_size = __get_total_component_length(n, components);

    struct cfs_result_size_t home_result = cfs_path_home_s(buffer, max_buffer_size);
    if (!home_result.info.is_error)
    {
        *result = home_result;
        return;
    }

    max_buffer_size -= strlen(buffer);
    if (max_buffer_size == 0)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        return;
    }

    __append_path_delim(buffer);
    max_buffer_size -= 1;

    struct cfs_result_size_t path_result;
    __create_full_path_s(&path_result, buffer, max_buffer_size, n, false, components);
    if (path_result.info.is_error)
    {
        *result = path_result;
        return;
    }

    if (strlen(buffer) >= required_buffer_size)
    {
        cfs_result_set_err_buffer_limit(&result->info);
        return;
    }

    __append_path_delim(buffer);
}

#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)
    static void __current_dir_win32_d(struct cfs_result_string_t* result)
    {
        DWORD buff_size = GetCurrentDirectory(0, NULL);
        char* buffer = calloc(buff_size + 1, sizeof(char));
        if (!buffer)
        {
            cfs_result_set_err_no_mem(&result->info);
            return;
        }

        if (GetCurrentDirectory(buff_size, buffer) == 0)
        {
            free(buffer);
            cfs_result_set_err_invalid_read(&result->info);
            cfs_result_message_write(&result->info, "Couldn't get current directory.\n");
            return;
        }

        result->value = buffer;
        cfs_result_set_success(&result->info);
    }

    static void __current_dir_win32_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size)
    {
        DWORD required_buffer_size = GetCurrentDirectory(0, NULL);
        if (max_buffer_size < (size_t)required_buffer_size)
        {
            cfs_result_set_err_buffer_limit(&result->info);
            result->value = required_buffer_size;
            return;
        }

        if (GetCurrentDirectory(max_buffer_size, buffer) == 0)
        {
            cfs_result_set_err_stdlib(&result->info);
            cfs_result_message_write(&result->info, "Couldn't retrieve current directory.\n");
            return;
        }

        cfs_result_set_success(&result->info);
    }
#else
    static void __current_dir_other_d(struct cfs_result_string_t* result)
    {
        char* path = getcwd(NULL, 0);
        if (!path)
        {
            cfs_result_set_err_no_mem(&result->info);
            return;
        }

        result->value = path;
        cfs_result_set_success(&result->info);
    }

    static void __current_dir_other_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size)
    {
        char* path = getcwd(buffer, max_buffer_size);
        if (!path)
        {
            cfs_result_set_err_buffer_limit(&result->info);
            result->value = 0;
            return;
        }

        cfs_result_set_success(&result->info);
        return;
    }
#endif

static void __vcreate_full_path_from_current_dir_d(struct cfs_result_string_t* result, size_t n, va_list path_components)
{
    struct cfs_result_string_t path_result;

    va_list path_components_copy;
    va_copy(path_components_copy, path_components);
    __vcreate_full_path_d(&path_result, true, n, path_components_copy);
    va_end(path_components_copy);

    if (path_result.info.is_error)
    {
        *result = path_result;
        return;
    }

    struct cfs_result_string_t current_dir_result;

    CFS_IMPL_WIN_OTHER(
    {
        __current_dir_win32_d(&current_dir_result);
    },
    {
        __current_dir_other_d(&current_dir_result);
    });

    if (current_dir_result.info.is_error)
    {
        *result = current_dir_result;
        return;
    }

    char* buffer = calloc(strlen(current_dir_result.value) + strlen(path_result.value) + 1, sizeof(char));
    if (!buffer)
    {
        cfs_result_set_err_no_mem(&result->info);
        return;
    }

    strcat(buffer, current_dir_result.value);
    strcat(buffer, path_result.value);

    result->value = buffer;
    cfs_result_set_success(&result->info);
}

static void __vcreate_full_path_from_current_dir_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size, size_t n, va_list path_components) 
{
    CFS_IMPL_WIN_OTHER(
    {
        __current_dir_win32_s(result, buffer, max_buffer_size);
    },
    {
        __current_dir_other_s(result, buffer, max_buffer_size);
    });

    if (result->info.is_error)
        return;

    max_buffer_size -= strlen(buffer);

    va_list path_components_copy;
    va_copy(path_components_copy, path_components);
    __vcreate_full_path_s(result, buffer, max_buffer_size, true, n, path_components_copy);
    va_end(path_components_copy);

    if (result->info.is_error)
        return;

    cfs_result_set_success(&result->info);
}

static void __create_full_path_from_current_dir_d(struct cfs_result_string_t* result, size_t n, const char** components)
{
    struct cfs_result_string_t path_result;

    __create_full_path_d(&path_result, true, n, components);

    if (path_result.info.is_error)
    {
        *result = path_result;
        return;
    }

    struct cfs_result_string_t current_dir_result;

    CFS_IMPL_WIN_OTHER(
    {
        __current_dir_win32_d(&current_dir_result);
    },
    {
        __current_dir_other_d(&current_dir_result);
    });

    if (current_dir_result.info.is_error)
    {
        *result = current_dir_result;
        return;
    }

    char* buffer = calloc(strlen(current_dir_result.value) + strlen(path_result.value) + 1, sizeof(char));
    if (!buffer)
    {
        cfs_result_set_err_no_mem(&result->info);
        return;
    }

    strcat(buffer, current_dir_result.value);
    strcat(buffer, path_result.value);

    result->value = buffer;
    cfs_result_set_success(&result->info);
}

static void __create_full_path_from_current_dir_s(struct cfs_result_size_t* result, char* buffer, size_t max_buffer_size, size_t n, const char** components)
{
    CFS_IMPL_WIN_OTHER(
    {
        __current_dir_win32_s(result, buffer, max_buffer_size);
    },
    {
        __current_dir_other_s(result, buffer, max_buffer_size);
    });

    if (result->info.is_error)
        return;

    max_buffer_size -= strlen(buffer);

    __create_full_path_s(result, buffer, max_buffer_size, true, n, components);

    if (result->info.is_error)
        return;

    cfs_result_set_success(&result->info);
}
