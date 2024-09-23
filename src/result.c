#include "result.h"
#include "result_internal.c"

void cfs_result_set_error(struct cfs_result_t* const result, bool is_error)
{
    if (!result)
        return;

    result->is_error = is_error;
}

void cfs_result_value_set_bool(struct cfs_result_t* const result, bool value)
{
    if (!result)
        return;

    result->value_type = ECFS_VALUE_TYPE_BOOL;
    result->value.as_bool = value;
}

void cfs_result_message_write_perror(struct cfs_result_t* const result)
{
    cfs_result_message_write(result, strerror(errno));
}

const char* cfs_result_message_get(const struct cfs_result_t* const result)
{
    if (!result)
        return NULL;

    return (const char*)&result->msg[0];
}

void cfs_result_message_write(struct cfs_result_t* const result, const char* const msg)
{
    if (!result || !msg)
        return;

    memset(result->msg, 0, CFS_MSG_SIZE);
    strncpy(&result->msg[0], msg, CFS_MSG_SIZE - 1);
}

void cfs_result_message_writef(struct cfs_result_t* const result, const char* const fmt, ...)
{
    if (!result || !fmt)
        return;

    va_list msg_list;
    va_start(msg_list, fmt);
    __write_messagef(result, fmt, msg_list);
    va_end(msg_list);
}

bool cfs_result_value_get_bool(const struct cfs_result_t* const result)
{
    if (!result)
        return false;

    return result->value.as_bool;
}
