#include "directory.h"
#include "util.h"

char* cfs_dir_get_home()
{
    CFS_IMPL_WIN_OTHER({
        strdup(getenv("HOME")); 
    },
    {
        char* home_drive = getenv("HOMEDRIVE");
        char* home_path = getenv("HOMEPATH");
        if (!home_drive || !home_path)
            return NULL;

        size_t home_size = strlen(home_drive) + strlen(home_path) + 1;
        char* home = calloc(home_size, sizeof(char));
        if (!home)
            return NULL;
            
        strcat(home, home_drive);
        strcat(home, home_path);

        return home;
    });
}
