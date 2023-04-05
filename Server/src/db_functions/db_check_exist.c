#include "../../inc/server.h"

int db_check_exist()
{
    struct stat info;

    if (stat(DB_NAME, &info) == 0)
        return 1;

    return 0;
}
