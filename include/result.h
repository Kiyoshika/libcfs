#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define CFS_MSG_SIZE 256

/**
 * Different return data types that can be set/fetched from a result.
 */
enum value_type_e
{
    ECFS_VALUE_TYPE_NONE = 0,
    ECFS_VALUE_TYPE_BOOL
};

/**
 * This is a result tuple for error handling.
 * A function that returns cfs_result_t can set is_error with an optional msg.
 *
 * When consuming these functions, check for errors with cfs_result_iserror(&result).
 * Obtain a message with cfs_result_message_get(&result) which returns a (read-only) char*.
 *
 * For successes, different value types can be stored such as bools, ints, etc. The function documentation
 * should specify what data is returned on success, but you can use any of the getters such as cfs_result_get_bool(&result).
 * It's also possible a function may not return data and only uses this as a convenient way to store error messages.
 */
struct cfs_result_t
{
    bool is_error;
    enum value_type_e value_type;
    union value
    {
        bool as_bool;
    } value;
    char msg[CFS_MSG_SIZE];
};

/**
 * Set whether or not the result is an error (i.e., there was a problem processing something.)
 * This is analogous to an exception being thrown.
 */
void cfs_result_set_error(struct cfs_result_t* const result, bool is_error);

/**
 * Sets a boolean value for the result content.
 */
void cfs_result_value_set_bool(struct cfs_result_t* const result, bool value);

/**
 * Get the result value as a bool.
 */
bool cfs_result_value_get_bool(const struct cfs_result_t* const result);

/**
 * Returns a const pointer to the message contained in the result.
 */
const char* cfs_result_message_get(const struct cfs_result_t* const result);

/**
 * Automatically write the errno message from strerror(errno) into msg within cfs_result_t.
 * Used when dealing with functions from the C standard library to wrap them in cfs_result_t.
 */
void cfs_result_message_write_perror(struct cfs_result_t* const result);

/**
 * Writes [msg] into msg up to CFS_MSG_SIZE - 1 bytes.
 */
void cfs_result_message_write(struct cfs_result_t* const, const char* const msg);

/**
 * Writes a formatted message into msg according to format [fmt] up to CFS_MSG_SIZE - 1 bytes.
 */
void cfs_result_message_writef(struct cfs_result_t* const, const char* const fmt, ...);
