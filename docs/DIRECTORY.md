# Directory API
This is the API for handling directories - from checking existence, creating, removing, renaming, etc.

## Header
```c
#include "directory.h"
```

## Content
* [Check directory exists](#check-directory-exists)
* [Create directory](#create-directory)
* [Remove directories recursively](#remove-directories-recursively)
* [Rename directory](#rename-directory)

### Check directory exists
This checks if an existing path is a directory. If the path doesn't exist at all, `is_error` is set to true.

If the path exists but is not a directory, the result will be false.

This returns a `cfs_result_t`; see the entry on [result_tupes](./RESULT.md) for more information on how to use this.

```c
#include "directory.h"
#include "path.h" // cfs_get_home()
#include "result.h" // struct cfs_result_t

struct cfs_result_t result = cfs_dir_exists("/some/fake/dir");

// this is an error because the path doesn't exist at all
assert(result.is_error == true);

char* home = cfs_get_home();
result = cfs_dir_exists(home);

assert(result.is_error == false);
assert(cfs_result_value_get_bool(&result) == true); // valid path + is a directory
free(home);

// assume below is a valid, existing path with a file.txt inside
result = cfs_dir_exists("/existing/path/with/file.txt");
assert(result.is_error == false);
assert(cfs_result_value_get_bool(&result) == false); // valid path but NOT a directory; it's a file
```
