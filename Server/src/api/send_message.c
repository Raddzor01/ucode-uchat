#include "../../inc/server.h"

void insert_message_into_db(cJSON *json);
int get_last_message_id(int user_id, int chat_id);

void send_message(cJSON *json, t_client_info *client_info) {

    int user_id;
    int chat_id;
    int message_id;
    cJSON *responde_json;
    char *json_str;

    user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

    insert_message_into_db(json);

    message_id = get_last_message_id(user_id, chat_id);

    responde_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(responde_json, "message_id", message_id);
    cJSON_AddNumberToObject(responde_json, "type", REQ_SEND_MSG);
    cJSON_AddNumberToObject(responde_json, "error_code", ERR_SUCCESS);
    json_str = cJSON_PrintUnformatted(responde_json);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(responde_json);

}

void insert_message_into_db(cJSON *json) {

    int user_id;
    int chat_id;
    char *message;
    unsigned long time;
    char *query;

    user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
    time = cJSON_GetObjectItemCaseSensitive(json, "time")->valueint;

    query = sqlite3_mprintf("INSERT INTO messages (user_id, chat_id, message, time) VALUES('%d', '%d', %Q, '%d')",
                            user_id, chat_id, message, time);
    db_execute_query(query);

    sqlite3_free(query);

}

int get_last_message_id(int user_id, int chat_id) {

    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query;
    int message_id;

    db = db_open();
    query = sqlite3_mprintf("SELECT id FROM messages WHERE user_id = '%d' AND chat_id = '%d' "
                            "ORDER BY id DESC LIMIT 1",
                            user_id, chat_id);
    stmt = db_execute_query_and_return_stmt(query, db);

    message_id = sqlite3_column_int(stmt, 0);

    sqlite3_free(query);
    sqlite3_close(db);
    sqlite3_finalize(stmt);

    return message_id;

}
