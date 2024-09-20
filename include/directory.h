#pragma once

#include <string.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * Returns the current user's home directory as a heap-allocated string.
 * Reads from HOMEDRIVE + HOMEPATH for windows systems, otherwise reads from HOME environment variables.
 * Returns NULL if memory allocation fails or if there was an issue reading an environment variable.
 */
char* cfs_dir_get_home();
