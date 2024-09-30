#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#define CFS_MSG_SIZE 256

enum error_type_e
{
    // no error
    ECFS_ERR_NONE = 0,
    // expected data from pointer but NULL was found
    ECFS_ERR_INVALID_READ,
    // attempted to allocate memory and failed
    ECFS_ERR_NO_MEM,
    // attempted to write data to pointer but NULL was found
    ECFS_ERR_INVALID_WRITE,
    // reached end of maximum buffer size while writing data
    ECFS_ERR_BUFFER_LIMIT,
    // bad argument value
    ECFS_ERR_INVALID_ARG,
    // an error coming from the standard library (when errno is set)
    ECFS_ERR_STDLIB
};

struct cfs_result_t
{
    bool is_error;
    enum error_type_e error_type;
    char msg[CFS_MSG_SIZE];
};

struct cfs_result_bool_t
{
    struct cfs_result_t info;
    bool value;
};

struct cfs_result_string_t
{
    struct cfs_result_t info;
    char* value;
};

struct cfs_result_size_t
{
    struct cfs_result_t info;
    size_t value;
};

/**
 * Free the memory pointed to by cfs_result_string_t
 */
void cfs_result_string_free(struct cfs_result_string_t* result);

/**
 * Set is_error = false and set error_type to ECFS_ERR_NONE to indicate
 * a success.
 */
void cfs_result_set_success(struct cfs_result_t* result);

/**
 * Set is_error = true and set error_type to ECFS_ERR_INVALID_READ.
 * 
 * Provides default message "Attempted to read data from pointer but got NULL." but can be overwritten.
 */
void cfs_result_set_err_invalid_read(struct cfs_result_t* result);

/**
 * Set is_error = true and set error_type to ECFS_ERR_NO_MEM.
 *
 * Provides default message "Couldn't allocate enough memory." but can be overwritten.
 */
void cfs_result_set_err_no_mem(struct cfs_result_t* result);

/**
 * Set is_error = true and set error_type to ECFS_ERR_INVALID_WRITE.
 *
 * Provides default message "Attempted to write data to pointer but got NULL." but can be overwritten.
 */
void cfs_result_set_err_invalid_write(struct cfs_result_t* result);

/**
 * Set is_error = true and set error_type to ECFS_ERR_BUFFER_LIMIT.
 *
 * Provides default message "Reached end of maximum buffer size while writing data." but can be overwritten.
 */
void cfs_result_set_err_buffer_limit(struct cfs_result_t* result);

/**
 * Set is_error = true and set error_type to ECFS_ERR_STDLIB.
 * 
 * Writes message from strerror(errno) but can be overwritten.
 */
void cfs_result_set_err_stdlib(struct cfs_result_t* result);

/**
 * Set is_error = true and set error_type to ECFS_ERR_INVALID_ARG.
 *
 * Provides default message "Invalid argument value." but can be overwritten.
 */
void cfs_result_set_err_invalid_arg(struct cfs_result_t* result);

/**
 * Returns a const pointer to the message contained in the result.
 */
const char* cfs_result_message_get(const struct cfs_result_t* result);

/**
 * Automatically write the errno message from strerror(errno) into msg within cfs_result_t.
 * Used when dealing with functions from the C standard library to wrap them in cfs_result_t.
 */
void cfs_result_message_write_perror(struct cfs_result_t* result);

/**
 * Writes [msg] into msg up to CFS_MSG_SIZE - 1 bytes.
 */
void cfs_result_message_write(struct cfs_result_t* const, const char* const msg);

/**
 * Writes a formatted message into msg according to format [fmt] up to CFS_MSG_SIZE - 1 bytes.
 */
void cfs_result_message_writef(struct cfs_result_t* const, const char* const fmt, ...);
