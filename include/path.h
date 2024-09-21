#pragma once

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

#include "directory.h"

/**
 * Get the home path for current user.
 * Returns a heap-allocated string with the home path or NULL on failure.
 */
char* cfs_path_home();

/**
 * Creates an arbitrary path with a trailing '/' or '\' (windows).
 *
 * Ex: vcfs_path_dir(3, "path", "to", "something")
 *     UNIX: /path/to/something
 *     WINDOWS: \path\to\something
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* vcfs_path_dir(size_t n, ...);

/**
 * Creates an arbitrary path WITHOUT a trailing '/' or '\' (windows) used for files.
 *
 * Ex: vcfs_path_file(4, "path", "to", "something", "file.txt");
 *     UNIX: /path/to/something/file.txt
 *     WINDOWS: \path\to\something\file.txt
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* vcfs_path_file(size_t n, ...);

/**
 * Creates an arbitrary path from user's home directory with a trailing '/' or '\' (windows).
 *
 * Ex: vcfs_path_dir_from_home(3, "path", "from", "home");
 *     UNIX: /home/<user>/path/from/home/
 *     WINDOWS: C:\Users\<user>\path\from\home\
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* vcfs_path_dir_from_home(size_t n, ...);

/**
 * Creates an arbitrary path from user's home directory WITHOUT a trailing '/' or '\' (windows).
 * Ex: vcfs_path_file_from_home(4, "path", "from", "home", "file.txt")
 *     UNIX: /home/<user>/path/from/home/file.txt
 *     WINDOWS: C:\Users\<user>\path\from\home\file.txt
 */
char* vcfs_path_file_from_home(size_t n, ...);
