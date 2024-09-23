# Result API
In some situations we want to return some data but the underlying process might fail - especially when dealing with the file system.

To handle these errors, we have a `struct cfs_result_t` to check for errors, obtain data and messages from such functions.

This error handling is analogous to "throwing exceptions", where if something fails, an error flag will be set along with a message we can read and print out to stdout or stderr for debugging/information.

## Checking For Errors
Typically a function will return a `cfs_result_t` which we can use to check errors:
```c
#include "path.h"   // cfs_path_exists()
#include "result.h" // cfs_result_t

struct cfs_result_t result = cfs_path_exists("/some/fake/path");
if (result.is_error)
{
    printf("Oh no! Got error: %s\n", result.msg);
    return -1;
}
else
{
    bool path_exists = cfs_result_value_get_bool(&result);
    assert(path_exists == false); // /some/fake/path doesn't exist
}
```

In general, we can use `cfs_result_value_get_<type>` where `<type>` is any supported type such as `bool`, `char*`, etc. Check the [header](../../include/path.h) for the full supported types.

Under the hood, the data is stored as a union. If the functions are too verbose for you, you can alternatively use `result.value.as_<type>` which is equivalent to `cfs_result_value_get_<type>(&result)`.

Keep in mind, values are **only** set if `is_error == false`. Otherwise, it will likely be garbage/uninitialized data.
