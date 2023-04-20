#include "../../inc/server.h"

int db_execute_query(const char *sql_query)
{
    sqlite3 *db = db_open();

    char *error;
    if (sqlite3_exec(db, sql_query, NULL, NULL, &error) != SQLITE_OK)
    {
        mx_logs(error, LOG_ERROR);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
