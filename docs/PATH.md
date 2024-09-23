# Path API
This is the API for handling paths - from checking existence, getting home directory and constructing arbitrary paths.

## Header
```c
#include "path.h"
```

## Content
* [Checking if a path exists](#checking-if-a-path-exists)
* [Getting the home directory](#getting-the-home-directory)
* [Creating paths](#creating-paths)
* [Creating paths from home directory](#creating-paths-from-home-directory)

### Checking if a path exists
This checks if a general path exists regardless of if it's a file or directory.

Simply provide a path as a `const char* const` to `cfs_path_exists()` which returns a `struct cfs_result_t`.

See the entry on [result types](./RESULT.md) for more information on how to use this.

```c
#include "path.h"   // cfs_path_exists()
#include "result.h" // struct cfs_result_t

struct cfs_result_t result = cfs_path_exists("/some/fake/path");

if (result.is_error)
{
    printf("%s\n", result.msg);
    return -1;
}
else
{
    bool path_exists = cfs_result_value_get_bool(&result);

    if (path_exists)
        printf("The path exists.\n");
    else
        printf("The path DOES NOT exist.\n");
}
```

### Getting the home directory
Get the home directory as a heap-allocated `char*`. Returns `NULL` on failure.

Note that YOU have ownership of this pointer, so you're responsible for freeing it afterwards.

```c
#include "path.h"

char* home = cfs_path_home();

if (!home)
    printf("Couldn't get HOME.\n");
else
{
    printf("HOME: %s\n", home);
    free(home);
}
```

### Creating paths
This will construct an arbitrary path delimited by `/` on unix or `\` on windows.

**NOTE:** This will NOT create any files or directories - only creates the *path*. If you want to actually *create* these things, look at the [file](FILE.md) or [directory](DIRECTORY.md) API.

This returns a heap-allocated `char*` that you must free afterwards.

You can create paths one of two ways:
* With variable arguments - useful if you know the path ahead of time
    * These functions are prefixed with a `v` to indicate variable arguments
* With a `char**` dynamic array of `char*` - useful if the path is dynamically generated at run time

And finally, there are two additional options:
* Create directory paths - adds a trailing `/` or `\` on windows
* Create file paths - ignores trailing delimiter

#### Variable Arguments
You can construct a path with variable arguments as follows:
```c
/*
    /some/random/path/ (UNIX)
    \some\random\path\ (WINDOWS)
*/
char* dir_path = vcfs_path_dir(3, "some", "random", "path");
free(dir_path);

/*
    /some/random/path/ (UNIX)
    \some\random\path\ (WINDOWS)
*/
char* file_path = vcfs_path_file(3, "some", "random", "file.txt");
free(file_path);
```

Notice that `_dir` appends the trailing slash while `_file` doesn't. Be sure to use the appropriate one for your use case.

#### Dynamic Array
You can construct a path with a dynamic array of `char*` as follows:

This is the same example as above but using a `char**` instead of variable arguments.

Note that `cfs_path_...` takes a `const char** const` so you may need to cast it to avoid compiler warnings.
```c
char** dynamic_path = calloc(3, sizeof(char*));
dynamic_path[0] = strdup("some");
dynamic_path[1] = strdup("random");
dynamic_path[2] = strdup("path");

/*
    /some/random/path/ (UNIX)
    \some\random\path\ (WINDOWS)
*/
char* dir_path = cfs_path_dir(3, (const char** const)dynamic_path);
free(dir_path);

free(dynamic_path[2]);
dynamic_path[2] = strdup("file.txt");

/*
    /some/random/file.txt (UNIX)
    \some\random\file.txt (WINDOWS)
*/
char* file_path = cfs_path_file(3, (const char** const)dynamic_path);
free(file_path);

free(dynamic_path[0]);
free(dynamic_path[1]);
free(dynamic_path[2]);
free(dynamic_path);
```

### Creating paths from home directory
The exact same rules and conditions apply from [here](#creating-paths) with the only difference being that the paths constructed start from the user's home directory. Like above, this supports the variable arguments or dynamic array approach:

#### Variable Arguments
```c
/*
    /home/<user>/path/from/home/ (LINUX)
    /Users/<user>/path/from/home/ (MACOS)
    C:\Users\<user>\path\from\home\ (WINDOWS)
*/
char* dir_path = vcfs_path_dir_from_home(3, "path", "from", "home");
free(dir_path);

/*
    /home/<user>/file/from/home.txt (LINUX)
    /Users/<user>/file/from/home.txt (MACOS)
    C:\Users\<user>\file\from\home.txt (WINDOWS)
*/
char* file_path = vcfs_path_file_from_home(3, "file", "from", "home.txt");
free(file_path);
```

#### Dynamic Array
Same example as above but using dynamic array.

Note that these functions use a `const char** const` so you may need to cast it to avoid compiler warnings.
```c
char** dynamic_path = calloc(3, sizeof(char*));
dynamic_path[0] = strdup("path");
dynamic_path[1] = strdup("from");
dynamic_path[2] = strdup("home");

/*
    /home/<user>/path/from/home/ (LINUX)
    /Users/<user>/path/from/home/ (MACOS)
    C:\Users\<user>\path\from\home\ (WINDOWS)
*/
char* dir_path = cfs_path_dir_from_home(3, (const char** const)dynamic_path);
free(dir_path);

free(dynamic_path[2]);
dynamic_path[2] = strdup("home.txt");

/*
    /home/<user>/file/from/home.txt (LINUX)
    /Users/<user>/file/from/home.txt (MACOS)
    C:\Users\<user>\file\from\home.txt (WINDOWS)
*/
char* file_path = cfs_path_file_from_home(3, (const char** const)dynamic_path);
free(file_path);

free(dynamic_path[0]);
free(dynamic_path[1]);
free(dynamic_path[2]);
free(dynamic_path);
```
