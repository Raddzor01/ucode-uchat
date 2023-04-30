#include "../../inc/server.h"

void edit_chat_name(cJSON *json, t_client_info *client_info)
{
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;

    char *query = NULL;
    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    char *new_name = cJSON_GetObjectItemCaseSensitive(json, "new_name")->valuestring;

    if(!db_check_chat_exists(chat_id))
    {
        send_responde(client_info->ssl, REQ_EDIT_CHAT_NAME, ERR_CHAT_NONEXIST);
        return;
    }

    if(!db_check_chat_membership(chat_id, client_info->user->id))
    {
        send_responde(client_info->ssl, REQ_EDIT_CHAT_NAME, ERR_USER_NOT_IN_CHAT);
        return;
    }

    db = db_open();
    query = sqlite3_mprintf("SELECT id FROM chats WHERE name = '%s'; ",
                            new_name);
    stmt = db_execute_query_and_return_stmt(query, db);

    if (sqlite3_column_int(stmt, 0) != 0)
    {
        send_responde(client_info->ssl, REQ_EDIT_CHAT_NAME, ERR_CHAT_EXIST);

        sqlite3_free(query);
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return;
    }

    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    query = sqlite3_mprintf("UPDATE chats SET name = '%s' WHERE id = %d; ",
                            new_name, chat_id);
    db_execute_query(query);

    sqlite3_free(query);

    send_responde(client_info->ssl, REQ_EDIT_CHAT_NAME, ERR_SUCCESS);
}
