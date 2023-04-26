#include "../../inc/server.h"

static cJSON *get_message(sqlite3_stmt *stmt)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "message_id", sqlite3_column_int(stmt, 0));
    cJSON_AddNumberToObject(json, "user_id", sqlite3_column_int(stmt, 1));
    cJSON_AddNumberToObject(json, "chat_id", sqlite3_column_int(stmt, 2));
    cJSON_AddStringToObject(json, "username", (const char *)sqlite3_column_text(stmt, 3));
    cJSON_AddNumberToObject(json, "image_id", sqlite3_column_int(stmt, 4));
    cJSON_AddStringToObject(json, "message", (const char *)sqlite3_column_text(stmt, 5));
    cJSON_AddNumberToObject(json, "date", sqlite3_column_int64(stmt, 6));
    return json;
}

static cJSON *get_messages_array(int chat_id)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;

    cJSON *messages_array = cJSON_CreateArray();

    db = db_open();
    query = sqlite3_mprintf("SELECT messages.id, messages.user_id, messages.chat_id, users.username, users.image_id, messages.message, messages.date FROM messages "
                            "INNER JOIN users ON users.id = messages.user_id "
                            "WHERE messages.chat_id = %d "
                            "ORDER BY messages.date; ",
                            chat_id);
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);


    while (sqlite3_step(stmt) == SQLITE_ROW)
        cJSON_AddItemToArray(messages_array, get_message(stmt));

    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return messages_array;
}

void get_chat_messages(cJSON *json, t_client_info *client_info)
{
    int chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;

    if (!db_check_chat_exists(chat_id))
    {
        send_responde(client_info->ssl, REQ_GET_CHAT_MESSAGES, ERR_CHAT_NONEXIST);
        return;
    }

    if (!db_check_chat_membership(chat_id, client_info->user->id))
    {
        send_responde(client_info->ssl, REQ_GET_CHAT_MESSAGES, ERR_USER_NOT_IN_CHAT);
        return;
    }

    cJSON *client_json = cJSON_CreateObject();
    cJSON *messages = get_messages_array(chat_id);
    cJSON_AddNumberToObject(client_json, "type", REQ_GET_CHAT_MESSAGES);
    cJSON_AddNumberToObject(client_json, "error_code", ERR_SUCCESS);
    cJSON_AddItemReferenceToObject(client_json, "messages", messages);
    char *json_str = cJSON_PrintUnformatted(client_json);

    // char *json_str_len = mx_itoa(mx_strlen(json_str)); 
    // SSL_write();
    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(client_json);
}
