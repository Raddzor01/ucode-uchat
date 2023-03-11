#include "../../inc/server.h"

void create_chat(cJSON *json, t_client_info *client_info) {

    db_init();

    sqlite3 *db;
    sqlite3_stmt *stmt;

    cJSON *chat_name = cJSON_GetObjectItemCaseSensitive(json, "name");
    cJSON *type = cJSON_GetObjectItem(json, "chat_type");
    cJSON *date = cJSON_GetObjectItem(json, "time");
    if (!cJSON_IsString(chat_name) || !cJSON_IsNumber(type) || !cJSON_IsNumber(date)) {
        send_responde(client_info->ssl, REQ_CREATE_CHAT, ERR_JSON);
        return;
    }
    db = db_open();

    sqlite3_prepare_v2(db, "SELECT id FROM chats WHERE name = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, chat_name->valuestring, -1, NULL);
    int chat_id = -1;
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        chat_id = sqlite3_column_int64(stmt, 0);
    }

    if(chat_id != -1) {
        send_responde(client_info->ssl, REQ_CREATE_CHAT, ERR_CHAT_EXIST);
    }

    sqlite3_reset(stmt);

    char *request = sqlite3_mprintf("INSERT INTO chats (name, type, date) VALUES('%s', '%d', '%d')", chat_name->valuestring, type->valueint, date->valueint);
    db_execute_request(request);
    sqlite3_free(request);

 
    request = sqlite3_mprintf("SELECT id FROM chats WHERE name = '%s'", chat_name->valuestring);

    sqlite3_prepare_v2(db, request, -1, &stmt, NULL);

    chat_id = sqlite3_column_int64(stmt, 0);

    sqlite3_free(request);
    sqlite3_reset(stmt);
    sqlite3_close(db);

    request = sqlite3_mprintf("INSERT INTO members (chat_id, user_id, privilege) VALUES('%d', '%d', '%d')", 
            chat_id, client_info->user->id, USERTYPE_ADMIN);
    db_execute_request(request);
    sqlite3_free(request);

}
