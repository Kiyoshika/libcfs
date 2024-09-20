# C File System Library - libcfs
I wanted to make working with files easier in C with some useful utilities related to constructing file paths.

I'm trying to make this library as cross platform as possible, but if something doesn't work, feel free to submit a PR.

## Features
* Get Current User's Home Directory
* Construct arbitrary directory/file paths
* Check if a path exists
* Check if a path is a file/directory
* Create/Remove/Rename directories
* Traverse a directory's contents

## Building
This is a typical CMake project with the following options:
* `-DDEBUG_MODE` (`OFF` by default)
* `-DCOMPILE_TESTS` (`OFF` by default)

```
mkdir build && cd build
cmake <options> ..
make        # build static library libcfs.a
make test   # run all tests
```

Or you can `add_subdirectory(src)` to this project in your own CMake file to include CFS in your build. Just add:
```
target_include_directories(<target> PUBLIC /path/to/libcfs/include)
target_link_libraries(<target> cfs)
```
