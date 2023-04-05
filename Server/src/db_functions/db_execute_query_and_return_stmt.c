#include "../../inc/server.h"

sqlite3_stmt *db_execute_query_and_return_stmt(const char *sql_query, sqlite3 *db)
{
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    sqlite3_step(stmt);

    return stmt;
}
