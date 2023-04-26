#include "../../inc/server.h"

void create_chat(cJSON *json, t_client_info *client_info)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *chat_name;
    int type;
    int date;
    int chat_id;
    char *query;

    chat_name = cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring;
    type = cJSON_GetObjectItem(json, "chat_type")->valueint;
    date = cJSON_GetObjectItem(json, "time")->valueint;

    db = db_open();
    query = sqlite3_mprintf("SELECT id FROM chats WHERE name = '%s'; ",
                            chat_name);
    stmt = db_execute_query_and_return_stmt(query, db);
    chat_id = sqlite3_column_int(stmt, 0);

    if (chat_id != 0)
    {
        send_responde(client_info->ssl, REQ_CREATE_CHAT, ERR_CHAT_EXIST);

        sqlite3_free(query);
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return;
    }

    sqlite3_free(query);
    sqlite3_finalize(stmt);

    query = sqlite3_mprintf("INSERT INTO chats (name, type, date) VALUES('%s', %d, %d); ",
                            chat_name, type, date);
    db_execute_query(query);

    sqlite3_free(query);

    query = sqlite3_mprintf("SELECT id FROM chats WHERE name = '%s'; ",
                            chat_name);
    stmt = db_execute_query_and_return_stmt(query, db);

    chat_id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_free(query);

    query = sqlite3_mprintf("INSERT INTO members (chat_id, user_id, privilege) VALUES(%d, %d, %d); ",
                            chat_id, client_info->user->id, PRIV_ADMIN);
    db_execute_query(query);

    cJSON *responde = cJSON_CreateObject();
    cJSON_AddNumberToObject(responde, "type", REQ_CREATE_CHAT);
    cJSON_AddNumberToObject(responde, "error_code", ERR_SUCCESS);
    cJSON_AddNumberToObject(responde, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(responde);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(responde);
    sqlite3_free(query);
    sqlite3_close(db);
}
