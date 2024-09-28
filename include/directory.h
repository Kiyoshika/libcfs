#pragma once

#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "result.h"

/**
 * Check if a path exists AND if it's a directory.
 * Will return false even if the path exists but is NOT a directory.
 *
 * On success, sets a bool value indicating the existence of a directory at the given path.
 */
struct cfs_result_t cfs_dir_exists(const char* const path);

/**
 * Create a new directory.
 *
 * Does not set a value on success.
 */
struct cfs_result_t cfs_dir_create(const char* const path);

/**
 * Recursively remove directory.
 *
 * Does not set a value on success.
 */
struct cfs_result_t cfs_dir_remove_all(const char* const path);

/**
 * Rename a directory.
 *
 * Does not set a value on success.
 */
struct cfs_result_t cfs_dir_rename(
    const char* const old_dir_path,
    const char* const new_dir_path);
