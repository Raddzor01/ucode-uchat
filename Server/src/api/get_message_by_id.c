#include "../../inc/server.h"

void get_message_by_id(cJSON *json, t_client_info *client_info)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;

    int chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;
    int message_id = cJSON_GetObjectItemCaseSensitive(json, "message_id")->valueint;

    db = db_open();
    query = sqlite3_mprintf("SELECT messages.id, messages.user_id, messages.chat_id, messages.message, messages.date, users.username, users.image_id FROM messages "
                            "INNER JOIN users ON users.id = messages.user_id "
                            "WHERE messages.chat_id = %d AND messages.id = %d; ",
                            chat_id, message_id);
    sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    cJSON *message_json = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        message_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(message_json, "message_id", sqlite3_column_int(stmt, 0));
        cJSON_AddNumberToObject(message_json, "user_id", sqlite3_column_int(stmt, 1));
        cJSON_AddNumberToObject(message_json, "chat_id", sqlite3_column_int(stmt, 2));
        cJSON_AddStringToObject(message_json, "message", (const char *)sqlite3_column_text(stmt, 3));
        cJSON_AddNumberToObject(message_json, "time", sqlite3_column_int64(stmt, 4));
        cJSON_AddStringToObject(message_json, "username", (const char *)sqlite3_column_text(stmt, 5));
        cJSON_AddNumberToObject(message_json, "image_id", sqlite3_column_int(stmt, 6));
    }

    cJSON *response = cJSON_CreateObject();
    cJSON_AddNumberToObject(response, "type", REQ_GET_MSG_BY_ID);
    cJSON_AddNumberToObject(response, "error_code", ERR_SUCCESS);
    cJSON_AddItemReferenceToObject(response, "message", message_json);
    char *json_str = cJSON_PrintUnformatted(response);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    cJSON_Delete(message_json);
    mx_strdel(&json_str);
}
