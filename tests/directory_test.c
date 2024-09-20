#include "directory.h"

#include <assert.h>

int main()
{
    // it's difficult to test this properly since this is dynamic depending on the user running this test
    // so we'll just ensure this isn't NULL
    char* home = cfs_dir_get_home();
    assert(home != NULL);

    free(home);
    return 0;
}
