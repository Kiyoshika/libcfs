#pragma once

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>

/* snippet from stack overflow: https://stackoverflow.com/a/62371749 */
// Windows does not define the S_ISREG and S_ISDIR macros in stat.h, so we do.
// We have to define _CRT_INTERNAL_NONSTDC_NAMES 1 before #including sys/stat.h
// in order for Microsoft's stat.h to define names like S_IFMT, S_IFREG, and S_IFDIR,
// rather than just defining  _S_IFMT, _S_IFREG, and _S_IFDIR as it normally does.
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#include <sys/stat.h>
#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
  #define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif
#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
  #define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#include <sys/types.h>

#include "directory.h"
#include "result.h"

/**
 * Check if a path to a directory OR file exists.
 * If you want more fine-grained checks, use cfs_dir_exists() or cfs_file_exists().
 *
 * Returns a cfs_result_t with a boolean value set for whether or not the path exists.
 */
struct cfs_result_t cfs_path_exists(const char* const path);

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
