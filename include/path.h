#pragma once

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include <sys/types.h>

#include "directory.h"
#include "result.h"
#include "util.h"

#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)
    #include <windows.h>
#else
    #include <unistd.h>
#endif

/**
 * Check if a path to a directory OR file exists.
 * If you want more fine-grained checks, use cfs_dir_exists() or cfs_file_exists().
 *
 * Stores a boolean whether the path exists or not on success.
 */
struct cfs_result_bool_t cfs_path_exists(const char* const path);

/**
 * Get the home path for current user.
 * Stores a heap-allocated char* containing the home path in the result on success.
 */
struct cfs_result_string_t cfs_path_home_d();

/**
 * Get the home path for the current user.
 *
 * Writes the home path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t cfs_path_home_s(char* buffer, size_t max_buffer_size);

/**
 * Creates an arbitrary path with a trailing '/' or '\' (windows) using variadic arguemnts.
 * Useful if you know the path ahead of time.
 *
 * Stores a heap-allocated char* containing the constructed path on success.
 */
struct cfs_result_string_t vcfs_path_dir_d(bool use_leading_delim, size_t n, ...);

/**
 * Creates an arbitrary path with a trailing '/' or '\' (windows) using variadic arguments.
 * Useful if you know the path ahead of time.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t vcfs_path_dir_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, ...);

/**
 * Creates an arbitrary path with a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if you need to generate the path dynamically at run time.
 *
 * Stores a heap-allocated char* containing the constructed path on success.
*/
struct cfs_result_string_t cfs_path_dir_d(size_t n, bool use_leading_delim, const char** components);

/**
 * Creates an arbitrary path with a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if you need to generate the path dynamically at run time.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t cfs_path_dir_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, const char** components); 

/**
 * Creates an arbitrary path WITHOUT a trailing '/' or '\' (windows) used for files using variadic arguments. 
 * Useful if you know the path ahead of time.
 *
 * Stores a heap-allocated char* containing the constructed path on success.
 */
struct cfs_result_string_t vcfs_path_file_d(bool use_leading_delim, size_t n, ...);

/**
 * Creates an arbitrary path WITHOUT a trailing '/' or '\' (windows) using a variadic arguments.
 * Useful if you know the path ahead of time.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t vcfs_path_file_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, ...);

/**
 * Creates an arbitrary path WITHOUT a trailing '/' or '\' (windows) used for files using a dynamic array of strings.
 * Usefil if you need to generate the path dynamically at run time.
 *
 * Stores a heap-allocated char* containing the constructed path on success.
 */
struct cfs_result_string_t cfs_path_file_d(bool use_leading_delim, size_t n, const char** components);

/**
 * Creates an arbitrary path WITHOUT a trailing '/' or '\' (windows) using a a dynamic array of strings.
 * Useful if you need to generate the path dynamically at run time.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t cfs_path_file_s(bool use_leading_delim, char* buffer, size_t max_buffer_size, size_t n, const char** components);

/**
 * Creates an arbitrary path starting from user's home directory with a trailing '/' or '\' (windows) using variadic arguemnts.
 * Useful if you know the path ahead of time.
 *
 * Stores a heap-allocated char* containing the constructed path on success.
 */
struct cfs_result_string_t vcfs_path_dir_from_home_d(size_t n, ...);

/**
 * Creates an arbitrary path starting from the user's home directory with a trailing '/' or '\' (windows) using variadic arguments.
 * Useful if you know the path ahead of time.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t vcfs_path_dir_from_home_s(char* buffer, size_t max_buffer_size, size_t n, ...);

/**
 * Creates an arbitrary path starting from user's home directory with a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if need to generate the path dynamically at run time.
 *
 * Stores a heap-allocated char* containing the constructed path on success.
 */
struct cfs_result_string_t cfs_path_dir_from_home_d(size_t n, const char** components);

/**
 * Creates an arbitrary path starting from the user's home directory with a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if you need to generate the path dynamically at run time.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t cfs_path_dir_from_home_s(char* buffer, size_t max_buffer_size, size_t n, ...);

/**
 * Creates an arbitrary path starting from user's home directory WITHOUT a trailing '/' or '\' (windows) using variadic arguments.
 * Useful if you know the exact path ahead of time.
 *
 * Stores a heap-allocated char* containing the constructed path on success.
 */
struct cfs_result_string_t vcfs_path_file_from_home_d(size_t n, ...);

/**
 * Creates an arbitrary path starting from the user's home directory WITHOUT a trailing '/' or '\' (windows) using variadic arguments.
 * Useful if you know the path ahead of time.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t vcfs_path_file_from_home_s(char* buffer, size_t max_buffer_size, size_t n, ...);

/**
 * Creates an arbitrary path starting from user's home directory WITHOUT a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if you need to generate the path dynamically at run time.
 *
 * Stores a heap-allocated char* containing the constructed path on success.
 */
struct cfs_result_string_t cfs_path_file_from_home_d(size_t n, const char** components);

/**
 * Creates an arbitrary path starting from the user's home directory WITHOUT a trailing '/' or '\' (windows) using a dynamic array of strings.
 * Useful if you need to generate the path dynamically at run time.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t cfs_path_file_from_home_s(char* buffer, size_t max_buffer_size, size_t n, ...);

/**
 * Get the current working directory.
 *
 * Stores a heap-allocated char* containing the current directory path on success.
 */
struct cfs_result_string_t cfs_path_current_dir_d();

/**
 * Get the current working directory.
 *
 * Writes the constructed path into [buffer] up to [max_buffer_size] bytes and appends a null terminator.
 *
 * If [buffer] is NULL, is_error is set to true with error_type = CFS_ERR_INVALID_WRITE
 *
 * If the provided buffer was not big enough, is_error is set to true with error_type = CFS_ERR_BUFFER_LIMIT 
 * and a size_t value is written containing the required buffer size.
 */
struct cfs_result_size_t cfs_path_current_dir_s(char* buffer, size_t max_buffer_size);
