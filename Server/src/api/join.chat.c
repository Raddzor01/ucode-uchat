#include "../../inc/server.h"

void join_chat(cJSON *chat_info, t_client_info *client_info)
{

    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query;
    char *chat_name;
    int chat_id;

    chat_name = cJSON_GetObjectItemCaseSensitive(chat_info, "chat_name")->valuestring;

    db = db_open();
    query = sqlite3_mprintf("SELECT id FROM chats WHERE name = '%s'",
                            chat_name);
    stmt = db_execute_query_and_return_stmt(query, db);
    chat_id = sqlite3_column_int(stmt, 0);

    if (chat_id == 0)
    {
        send_responde(client_info->ssl, REQ_JOIN_CHAT, ERR_CHAT_NONEXIST);

        sqlite3_finalize(stmt);
        sqlite3_free(query);
        sqlite3_close(db);

        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_free(query);

    query = sqlite3_mprintf("SELECT EXISTS (SELECT id FROM members WHERE user_id = '%d' AND chat_id = '%d')",
                            client_info->user->id, chat_id);
    stmt = db_execute_query_and_return_stmt(query, db);

    if (sqlite3_column_int(stmt, 0) == 1)
    {
        send_responde(client_info->ssl, REQ_JOIN_CHAT, ERR_USER_IN_CHAT);

        sqlite3_finalize(stmt);
        sqlite3_free(query);
        sqlite3_close(db);

        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_free(query);
    sqlite3_close(db);

    query = sqlite3_mprintf("INSERT INTO members (user_id, chat_id, permissions) VALUES('%d', '%d', '%d')",
                            client_info->user->id, chat_id, USERTYPE_NORMAL);
    db_execute_query(query);

    sqlite3_free(query);
}
