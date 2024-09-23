#include "result.h"

#include <assert.h>

int main()
{
    struct cfs_result_t result;

    // below should all be a NO-OP, otherwise they would segfault

    cfs_result_set_error(NULL, false);
    cfs_result_value_set_bool(NULL, false);
    cfs_result_value_get_bool(NULL);
    cfs_result_message_get(NULL);
    cfs_result_message_write_perror(NULL);

    cfs_result_message_write(NULL, NULL);
    cfs_result_message_write(&result, NULL);

    cfs_result_message_writef(NULL, NULL);
    cfs_result_message_writef(&result, NULL);

    // "real" tests

    cfs_result_set_error(&result, true);
    cfs_result_message_write(&result, "failure message");
    assert(result.is_error == true);
    assert(strcmp(cfs_result_message_get(&result), "failure message") == 0);

    cfs_result_message_writef(&result, "formatted '%s' %d", "message", 12);
    assert(strcmp(cfs_result_message_get(&result), "formatted 'message' 12") == 0);

    cfs_result_set_error(&result, false);
    assert(result.is_error == false);

    cfs_result_value_set_bool(&result, true);
    assert(result.value_type == ECFS_VALUE_TYPE_BOOL);
    assert(cfs_result_value_get_bool(&result) == true);

    return 0;
}
