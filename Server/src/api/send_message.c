#include "../../inc/server.h"

void send_message(cJSON *json, t_client_info *client_info) {

    db_init();

    sqlite3 *db;
    sqlite3_stmt *stmt;

    cJSON *user_id = cJSON_GetObjectItem(json, "user_id");
    cJSON *chat_id = cJSON_GetObjectItem(json, "chat_id");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    cJSON *date = cJSON_GetObjectItemCaseSensitive(json, "date");

    if (!cJSON_IsNumber(user_id) || !cJSON_IsNumber(chat_id) || !cJSON_IsString(message) || !cJSON_IsNumber(date)) {
        send_responde(client_info->ssl, REQ_SEND_MSG, ERR_JSON);
        return;
    }

    db = db_open();
    char request[MAX_BUF_SIZE];
    

}
