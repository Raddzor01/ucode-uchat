#include "../../inc/server.h"

int db_check_user_exists(char *username)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;
    bool check = false;

    query = sqlite3_mprintf("SELECT EXISTS (SELECT id FROM users WHERE username = '%s'); ",
                            username);
    
    db = db_open();
    stmt = db_execute_query_and_return_stmt(query, db);
    check = sqlite3_column_int64(stmt, 0);

    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return check;
}
