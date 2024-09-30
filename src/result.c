#include "result.h"
#include "result_internal.c"

#define CFS_SET_ERR(_is_error, _error_type, _message_str){ \
    result->is_error = _is_error; \
    result->error_type = _error_type; \
    memset(&result->msg[0], 0, CFS_MSG_SIZE); \
    strncat(&result->msg[0], _message_str, CFS_MSG_SIZE); \
}

void cfs_result_string_free(struct cfs_result_string_t* result)
{
    if (!result)
        return;

    free(result->value);
    result->value = NULL;
}

void cfs_result_set_success(struct cfs_result_t* result)
{
    CFS_SET_ERR(false, ECFS_ERR_NONE, "");
}

void cfs_result_set_err_invalid_read(struct cfs_result_t* result)
{
    CFS_SET_ERR(true, ECFS_ERR_INVALID_READ, "Attempted to read data from pointer but got NULL.\n");
}

void cfs_result_set_err_no_mem(struct cfs_result_t* result)
{
    CFS_SET_ERR(true, ECFS_ERR_NO_MEM, "Couldn't allocate enough memory.\n");
}

void cfs_result_set_err_invalid_write(struct cfs_result_t* result)
{
    CFS_SET_ERR(true, ECFS_ERR_INVALID_WRITE, "Attempted to write data to pointer but got NULL.\n");
}

void cfs_result_set_err_buffer_limit(struct cfs_result_t* result)
{
    CFS_SET_ERR(true, ECFS_ERR_BUFFER_LIMIT, "Reached end of maximum buffer size while writing data.\n");
}

void cfs_result_set_err_stdlib(struct cfs_result_t* result)
{
    CFS_SET_ERR(true, ECFS_ERR_STDLIB, strerror(errno));
}

void cfs_result_set_err_invalid_arg(struct cfs_result_t* result)
{
    CFS_SET_ERR(true, ECFS_ERR_INVALID_ARG, "Invalid argument value.\n");
}

void cfs_result_message_write_perror(struct cfs_result_t* result)
{
    cfs_result_message_write(result, strerror(errno));
}

const char* cfs_result_message_get(const struct cfs_result_t* result)
{
    if (!result)
        return NULL;

    return (const char*)&result->msg[0];
}

void cfs_result_message_write(struct cfs_result_t* result, const char* const msg)
{
    if (!result || !msg)
        return;

    memset(result->msg, 0, CFS_MSG_SIZE);
    strncpy(&result->msg[0], msg, CFS_MSG_SIZE - 1);
}

void cfs_result_message_writef(struct cfs_result_t* result, const char* const fmt, ...)
{
    if (!result || !fmt)
        return;

    va_list msg_list;
    va_start(msg_list, fmt);
    __write_messagef(result, fmt, msg_list);
    va_end(msg_list);
}
