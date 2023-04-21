#include "../../inc/server.h"

void insert_message_into_db(cJSON *json);

void send_message(cJSON *json, t_client_info *client_info)
{
    int user_id;
    unsigned long chat_id;
    int message_id;
    cJSON *responde_json;
    char *json_str;

    user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

    insert_message_into_db(json);

    message_id = db_get_last_message_id(user_id, chat_id);

    responde_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(responde_json, "message_id", message_id);
    cJSON_AddNumberToObject(responde_json, "type", REQ_SEND_MSG);
    cJSON_AddNumberToObject(responde_json, "error_code", ERR_SUCCESS);
    json_str = cJSON_PrintUnformatted(responde_json);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(responde_json);
}

void insert_message_into_db(cJSON *json)
{
    int user_id;
    int chat_id;
    char *message;
    time_t time;
    char *query;
    sqlite3_stmt *stmt;
    sqlite3 *db = db_open();

    user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
    time = cJSON_GetObjectItemCaseSensitive(json, "time")->valueint;

    query = sqlite3_mprintf("INSERT INTO messages (user_id, chat_id, message, date) VALUES(%d, %d, ?, %ld); ",
                            user_id, chat_id, time);
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, message, -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_free(query);

    query = sqlite3_mprintf("UPDATE chats SET date = %ld WHERE id = %d ; ",
                            time, chat_id);
    db_execute_query(query);
    
    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
