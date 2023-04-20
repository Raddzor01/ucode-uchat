#include "../../inc/server.h"

sqlite3 *db_open()
{
    sqlite3 *db;

    if (sqlite3_open(DB_NAME, &db) != SQLITE_OK)
    {
        mx_logs(strerror(errno), LOG_ERROR);
        return NULL;
    }

    return db;
}
