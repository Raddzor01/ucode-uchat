#include "../../inc/server.h"

bool db_check_chat_membership(int chat_id, int user_id)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;
    bool check = false;

    query = sqlite3_mprintf("SELECT EXISTS (SELECT 1 FROM members WHERE user_id = %d AND chat_id = %d); ", 
                            user_id, chat_id);
    
    db = db_open();
    stmt = db_execute_query_and_return_stmt(query, db);
    check = sqlite3_column_int(stmt, 0);

    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return check;
}
