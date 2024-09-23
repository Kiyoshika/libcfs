// internal functions to be included in the result.c source
// these forward declarations are only here for "documentation"
static void __write_messagef(struct cfs_result_t* const, const char* const, va_list);

/* IMPLEMENTATION */
static void __write_messagef(struct cfs_result_t* const result, const char* const fmt, va_list msg_list)
{
    if (!result)
        return;

    vsnprintf(result->msg, CFS_MSG_SIZE, fmt, msg_list);
}
