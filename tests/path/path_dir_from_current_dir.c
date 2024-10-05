#include "path.h"
#include "util.h"
#include <assert.h>

#define EXPECTED_WINDOWS_PATH "\\path\\from\\current_dir\\"
#define EXPECTED_UNIX_PATH "/path/from/current_dir/"

#define ASSERT_PATH_VARARGS_D(_result){\
    CFS_IMPL_WIN_OTHER(\
    {\
        char* buffer = calloc(strlen(home_result.value) + strlen(EXPECTED_WINDOWS_PATH) + 1, sizeof(char));\
        strcat(buffer, home_result.value);\
        strcat(buffer, EXPECTED_WINDOWS_PATH);\
        assert(strcmp(_result.value, buffer) == 0);\
        free(buffer);\
        buffer = calloc(strlen(home_buffer) + strlen(EXPECTED_WINDOWS_PATH) + 1, sizeof(char));\
        strcat(buffer, home_buffer);\
        strcat(buffer, EXPECTED_WINDOWS_PATH);\
        assert(strcmp(_result.value, buffer) == 0);\
        free(buffer);\
    },\
    {\
        char* buffer = calloc(strlen(home_result.value) + strlen(EXPECTED_UNIX_PATH) + 1, sizeof(char));\
        strcat(buffer, home_result.value);\
        strcat(buffer, EXPECTED_UNIX_PATH);\
        assert(strcmp(_result.value, buffer) == 0);\
        free(buffer);\
        buffer = calloc(strlen(home_buffer) + strlen(EXPECTED_UNIX_PATH) + 1, sizeof(char));\
        strcat(buffer, home_buffer);\
        strcat(buffer, EXPECTED_UNIX_PATH);\
        assert(strcmp(_result.value, buffer) == 0);\
        free(buffer);\
    });\
}

#define ASSERT_PATH_VARARGS_S(_local_buffer){\
    CFS_IMPL_WIN_OTHER(\
    {\
        char* buffer = calloc(strlen(home_result.value) + strlen(EXPECTED_WINDOWS_PATH) + 1, sizeof(char));\
        strcat(buffer, home_result.value);\
        strcat(buffer, EXPECTED_WINDOWS_PATH);\
        assert(strcmp(_local_buffer, buffer) == 0);\
        free(buffer);\
        buffer = calloc(strlen(home_buffer) + strlen(EXPECTED_WINDOWS_PATH) + 1, sizeof(char));\
        strcat(buffer, home_buffer);\
        strcat(buffer, EXPECTED_WINDOWS_PATH);\
        assert(strcmp(_local_buffer, buffer) == 0);\
        free(buffer);\
    },\
    {\
        char* buffer = calloc(strlen(home_result.value) + strlen(EXPECTED_UNIX_PATH) + 1, sizeof(char));\
        strcat(buffer, home_result.value);\
        strcat(buffer, EXPECTED_UNIX_PATH);\
        assert(strcmp(_local_buffer, buffer) == 0);\
        free(buffer);\
        buffer = calloc(strlen(home_buffer) + strlen(EXPECTED_UNIX_PATH) + 1, sizeof(char));\
        strcat(buffer, home_buffer);\
        strcat(buffer, EXPECTED_UNIX_PATH);\
        assert(strcmp(_local_buffer, buffer) == 0);\
        free(buffer);\
    });\
}

int main()
{
    char** dynamic_path_1 = calloc(3, sizeof(char*));
    dynamic_path_1[0] = strdup("a");
    dynamic_path_1[1] = strdup("b");
    dynamic_path_1[2] = strdup("");

    char** dynamic_path_2 = calloc(3, sizeof(char*));
    dynamic_path_2[0] = strdup("path");
    dynamic_path_2[1] = strdup("from");
    dynamic_path_2[2] = strdup("current_dir");

    struct cfs_result_string_t home_result = cfs_path_current_dir_d();
    char home_buffer[256] = {0};
    cfs_path_current_dir_s(home_buffer, 256);

    struct cfs_result_string_t result = vcfs_path_dir_from_current_dir_d(false, 0);
    assert(result.info.is_error == true);

    result = vcfs_path_dir_from_current_dir_d(false, 3, "a", "b", "");
    assert(result.info.is_error == true);
    assert(result.info.error_type == ECFS_ERR_INVALID_ARG);

    result = vcfs_path_dir_from_current_dir_d(3, "path", "from", "current_dir");
    assert(result.info.is_error == false);

    ASSERT_PATH_VARARGS_D(result);

    char local_buffer[256] = {0};
    struct cfs_result_size_t result2 = vcfs_path_dir_from_current_dir_s(local_buffer, 256, 3, "a", "b", "");
    assert(result2.info.is_error == true);
    
    result2 = vcfs_path_dir_from_current_dir_s(local_buffer, 256, 3, "path", "from", "current_dir");
    assert(result2.info.is_error == false);

    CFS_IMPL_WIN_OTHER(
    {
        char* buffer = calloc(strlen(home_result.value) + strlen(EXPECTED_WINDOWS_PATH) + 1, sizeof(char));
        strcat(buffer, home_result.value);
        strcat(buffer, EXPECTED_WINDOWS_PATH);
        assert(strcmp(local_buffer, buffer) == 0);
        free(buffer);
    },
    {
        char* buffer = calloc(strlen(home_result.value) + strlen(EXPECTED_UNIX_PATH) + 1, sizeof(char));
        strcat(buffer, home_result.value);
        strcat(buffer, EXPECTED_UNIX_PATH);
        assert(strcmp(local_buffer, buffer) == 0);
        free(buffer);
    });

    result = cfs_path_dir_from_current_dir_d(3, (const char**)dynamic_path_1);
    assert(result.info.is_error == true);

    result = cfs_path_dir_from_current_dir_d(3, (const char**)dynamic_path_2);
    assert(result.info.is_error == false);
    CFS_IMPL_WIN_OTHER(
    {
        char* buffer = calloc(strlen(home_result.value) + strlen(EXPECTED_WINDOWS_PATH) + 1, sizeof(char));
        strcat(buffer, home_result.value);
        strcat(buffer, EXPECTED_WINDOWS_PATH);
        assert(strcmp(result.value, buffer) == 0);
        free(buffer);
    },
    {
        char* buffer = calloc(strlen(home_result.value) + strlen(EXPECTED_UNIX_PATH) + 1, sizeof(char));
        strcat(buffer, home_result.value);
        strcat(buffer, EXPECTED_UNIX_PATH);
        assert(strcmp(result.value, buffer) == 0);
        free(buffer);
    });

    cfs_result_string_free(&home_result);

    return 0;
}
