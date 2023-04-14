#include "../../inc/server.h"

int db_get_last_message_id(int user_id, int chat_id)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;
    int message_id;

    db = db_open();
    query = sqlite3_mprintf("SELECT id FROM messages WHERE user_id = '%d' AND chat_id = '%d' "
                            "ORDER BY id DESC LIMIT 1; ",
                            user_id, chat_id);
    stmt = db_execute_query_and_return_stmt(query, db);

    message_id = sqlite3_column_int(stmt, 0);

    sqlite3_free(query);
    sqlite3_close(db);
    sqlite3_finalize(stmt);

    return message_id;
}
