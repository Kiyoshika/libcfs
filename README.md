# C File System Library - libcfs
I wanted to make working with files and directories easier in C.

I'm trying to make this library as cross platform as possible, but if something doesn't work, feel free to submit a PR.

## Documentation
Check out the work in progress [documentation](docs/) on how to use this library.

## Features
NOTE: some of these features are still in development

* Get Current User's Home Directory
* Construct arbitrary directory/file paths
* Check if a path exists
* Check if a path is a file/directory
* Check metadata such as last modified time on files/directories
* Create/Remove/Rename directories
* Traverse a directory's contents
* Overwrite/append files
* Read or stream files

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
