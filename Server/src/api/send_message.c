#include "../../inc/server.h"

void insert_message_into_db(cJSON *json);
int get_last_message_id(int user_id, int chat_id);

void send_message(cJSON *json, t_client_info *client_info) {

    db_init();

    int user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

    insert_message_into_db(json);

    int message_id = get_last_message_id(user_id, chat_id);

    cJSON* responde_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(responde_json, "message_id", message_id);
    cJSON_AddNumberToObject(responde_json, "type", REQ_SEND_MSG);
    cJSON_AddNumberToObject(responde_json, "error_code", ERR_SUCCESS);
    char *json_str = cJSON_PrintUnformatted(responde_json);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(responde_json);

}

void insert_message_into_db(cJSON *json) {

    sqlite3 *db;
    db = db_open();

    int user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
    unsigned long time = cJSON_GetObjectItemCaseSensitive(json, "time")->valueint;

    char *request = sqlite3_mprintf("INSERT INTO messages (user_id, chat_id, message, time) VALUES('%d', '%d', %Q, '%d')", user_id, chat_id, message, time);
    db_execute_request(request);

    sqlite3_free(request);
    sqlite3_close(db);

}

int get_last_message_id(int user_id, int chat_id) {

    sqlite3 *db;
    sqlite3_stmt *stmt;

    db = db_open();

    char *request = sqlite3_mprintf("SELECT id FROM messages WHERE user_id = '%d' AND chat_id = '%d' "
                                    "ORDER BY id DESC LIMIT 1",
                                    user_id, chat_id);
    sqlite3_prepare_v2(db, request, -1, &stmt, NULL);
    sqlite3_step(stmt);

    int message_id = sqlite3_column_int64(stmt, 0);

    sqlite3_free(request);
    sqlite3_close(db);
    sqlite3_finalize(stmt);

    return message_id;

}
