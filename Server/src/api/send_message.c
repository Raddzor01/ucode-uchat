#include "../../inc/server.h"

void send_message(cJSON *json, t_client_info *client_info) {

    db_init();

    sqlite3 *db;
    sqlite3_stmt *stmt;

    int user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
    char *username = cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring;
    unsigned long time = cJSON_GetObjectItemCaseSensitive(json, "time")->valueint;

    // if (!cJSON_IsNumber(user_id) || !cJSON_IsNumber(chat_id) || !cJSON_IsString(message) || !cJSON_IsNumber(date)) {
    //     send_responde(client_info->ssl, REQ_SEND_MSG, ERR_JSON);
    //     return;
    // }

    db = db_open();
    char *request = sqlite3_mprintf("INSERT INTO messages (user_id, chat_id, message, time) VALUES('%d', '%d', %Q, '%d')", user_id, chat_id, message, time);
    db_execute_request(request);
    sqlite3_free(request);

    sprintf(request,  "SELECT id FROM `messages` WHERE `user_id` = '%d' AND `chat_id` = '%d' "
                    "ORDER BY `id` DESC LIMIT 1",
            user_id, chat_id);
    sqlite3_prepare_v2(db, request, -1, &stmt, NULL);
    sqlite3_step(stmt);

    int message_id = sqlite3_column_int64(stmt, 0);

    cJSON* responde_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(responde_json, "message_id", message_id);
    cJSON_AddNumberToObject(responde_json, "type", REQ_SEND_MSG);
    cJSON_AddNumberToObject(responde_json, "error_code", ERR_SUCCESS);
    char* json_str = cJSON_PrintUnformatted(responde_json);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_logs(json_str, INFO_LOG);

    mx_strdel(&json_str);
    cJSON_Delete(responde_json);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}
