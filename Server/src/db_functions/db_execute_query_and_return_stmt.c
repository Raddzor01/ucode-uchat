#include "../../inc/server.h"

sqlite3_stmt *db_execute_query_and_return_stmt(const char *sql_query, sqlite3 *db)
{
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    int rc = sqlite3_step(stmt);
    if(rc != SQLITE_ROW && rc != SQLITE_DONE)
        mx_logs((char *)sqlite3_errmsg(db), LOG_ERROR);

    return stmt;
}
