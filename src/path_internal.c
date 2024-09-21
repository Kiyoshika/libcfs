// internal functions to be included in the main implementation file

// most of these internal functions have a varying implementation depending
// on whether it takes variadic args (prefixed with 'v') or a dynamic
// array of strings.
//
// these forward declarations are only here for "documentation" or to
// quickly glance the internal API

static size_t __vget_total_component_length(size_t, va_list);
static size_t __get_total_component_length(size_t, const char** const);

static char*  __valloc_path_buffer(size_t, va_list);
static char*  __alloc_path_buffer(size_t, const char** const);

static void   __append_path_delim(char*);

static char*  __vcreate_full_path(size_t, va_list);
static char*  __create_full_path(size_t, const char** const);

static char*  __vcreate_full_path_from_home(size_t, va_list);
static char*  __create_full_path_from_home(size_t, const char** const);

static inline char* __dir_get_home_win32();
static inline char* __dir_get_home_other();

/* IMPLEMENTATION */

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

static char* __vcreate_full_path(size_t n, va_list component_list)
{
    va_list path_component;
    va_list path_component_copy;
    va_copy(path_component, component_list);
    va_copy(path_component_copy, component_list);

    // use one iteration to allocate the path buffer
    char* path_buffer = __valloc_path_buffer(n, path_component_copy);
    va_end(path_component_copy);

    if (!path_buffer)
        return NULL;

    // use another iteration to construct the path
    __append_path_delim(path_buffer);

    for (size_t i = 0; i < n; ++i)
    {
        const char* component = va_arg(path_component_copy, const char*);
        if (strlen(component) == 0)
        {
            free(path_buffer);
            return NULL;
        }
        strcat(path_buffer, component);
        __append_path_delim(path_buffer);
    }
    va_end(path_component_copy);

    return path_buffer;
}

static char* __create_full_path(size_t n, const char** components)
{
    char* path_buffer = __alloc_path_buffer(n, components);
    if (!path_buffer)
        return NULL;

    __append_path_delim(path_buffer);

    for (size_t i = 0; i < n; ++i)
    {
        const char* component = components[i];
        if (strlen(component) == 0)
        {
            free(path_buffer);
            return NULL;
        }
        strcat(path_buffer, component);
        __append_path_delim(path_buffer);
    }

    return path_buffer;
}

static inline char* __dir_get_home_other()
{
    return strdup(getenv("HOME"));
}

static inline char* __dir_get_home_win32()
{
    char* home_drive = getenv("HOMEDRIVE");
    char* home_path = getenv("HOMEPATH");
    if (!home_drive || !home_path)
        return NULL;

    size_t home_size = strlen(home_drive) + strlen(home_path) + 1;
    char* home = calloc(home_size, sizeof(char));
    if (!home)
        return NULL;
        
    strcat(home, home_drive);
    strcat(home, home_path);

    return home;
}

static char* __vcreate_full_path_from_home(size_t n, va_list component_list)
{
    char* home = cfs_path_home();
    if (!home || strlen(home) == 0)
    {
        free(home);
        return NULL;
    }

    // remove trailing '/' or '\' at end of home (if present)
    // since it will be added back when appending path
    size_t end_home = strlen(home) - 1;
    if (home[end_home] == '/' || home[end_home] == '\\')
        home[end_home] = '\0';

    char* path = __vcreate_full_path(n, component_list);
    if (!path)
    {
        free(home);
        return NULL;
    }

    size_t len = strlen(home) + strlen(path) + 1;
    char* full_path = calloc(len, sizeof(char));
    if (!full_path)
    {
        free(home);
        free(path);
        return NULL;
    }

    strcat(full_path, home);
    strcat(full_path, path);

    free(home);
    free(path);
    return full_path;
}

static char* __create_full_path_from_home(size_t n, const char** const components)
{
    char* home = cfs_path_home();
    if (!home || strlen(home) == 0)
    {
        free(home);
        return NULL;
    }

    // remove trailing '/' or '\' at end of home (if present)
    // since it will be added back when appending path
    size_t end_home = strlen(home) - 1;
    if (home[end_home] == '/' || home[end_home] == '\\')
        home[end_home] = '\0';

    char* path = __create_full_path(n, components);
    if (!path)
    {
        free(home);
        return NULL;
    }

    size_t len = strlen(home) + strlen(path) + 1;
    char* full_path = calloc(len, sizeof(char));
    if (!full_path)
    {
        free(home);
        free(path);
        return NULL;
    }

    strcat(full_path, home);
    strcat(full_path, path);

    free(home);
    free(path);
    return full_path;
}
