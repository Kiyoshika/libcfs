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
 * Creates an arbitrary path with a trailing '/' or '\' (windows) using variadic arguemnts.
 * Useful if you know the exact path ahead of time.
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* vcfs_path_dir(size_t n, ...);

/**
 * Creates an arbitrary path with a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if you need to generate the path dynamically at run time.
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
*/
char* cfs_path_dir(size_t n, const char** const components);

/**
 * Creates an arbitrary path WITHOUT a trailing '/' or '\' (windows) used for files using variadic arguments. 
 * Useful if you know the exact path ahead of time.
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* vcfs_path_file(size_t n, ...);

/**
 * Creates an arbitrary path WITHOUT a trailing '/' or '\' (windows) used for files using a dynamic array of strings.
 * Usefil if you need to generate the path dynamically at run time.
 *
 * Returns a heap-allocated char* containing the constructed path or NULL or failure.
 */
char* cfs_path_file(size_t n, const char** const components);

/**
 * Creates an arbitrary path starting from user's home directory with a trailing '/' or '\' (windows) using variadic arguemnts.
 * Useful if you know the exact path ahead of time.
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* vcfs_path_dir_from_home(size_t n, ...);

/**
 * Creates an arbitrary path starting from user's home directory with a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if need to generate the path dynamically at run time.
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* cfs_path_dir_from_home(size_t n, const char** const components);

/**
 * Creates an arbitrary path starting from user's home directory WITHOUT a trailing '/' or '\' (windows) using variadic arguments.
 * Useful if you know the exact path ahead of time.
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* vcfs_path_file_from_home(size_t n, ...);

/**
 * Creates an arbitrary path starting from user's home directory WITHOUT a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if you need to generate the path dynamically at run time.
 *
 * Returns a heap-allocated char* containing the constructed path or NULL on failure.
 */
char* cfs_path_file_from_home(size_t n, const char** const components);
