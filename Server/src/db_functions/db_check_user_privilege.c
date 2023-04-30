#include "../../inc/server.h"

t_privilege_type db_check_user_privilege(int user_id, int chat_id)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;
    t_privilege_type privilege;

    query = sqlite3_mprintf("SELECT privilege FROM members WHERE user_id = %d AND chat_id = %d; ",
                            user_id, chat_id);

    db = db_open();
    stmt = db_execute_query_and_return_stmt(query, db);
    privilege = sqlite3_column_int(stmt, 0);

    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return privilege;
}
