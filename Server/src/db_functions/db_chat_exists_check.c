#include "../../inc/server.h"

bool db_check_chat_exists(int id)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;
    bool check = false;

    query = sqlite3_mprintf("SELECT EXISTS (SELECT id FROM chats WHERE id = '%d'); ",
                            id);

    db = db_open();
    stmt = db_execute_query_and_return_stmt(query, db);
    check = sqlite3_column_int(stmt, 0);

    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return check;
}
