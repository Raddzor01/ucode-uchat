#include "../../inc/server.h"

void create_chat(cJSON *json, t_client_info *client_info) {

    sqlite3 *db;
    sqlite3_stmt *stmt;

    char *chat_name = cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring;
    int type = cJSON_GetObjectItem(json, "chat_type")->valueint;
    int date = cJSON_GetObjectItem(json, "time")->valueint;

    db = db_open();

    sqlite3_prepare_v2(db, "SELECT id FROM chats WHERE name = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, chat_name, -1, NULL);
    int chat_id = -1;
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        chat_id = sqlite3_column_int64(stmt, 0);
    }

    if(chat_id != -1) {
        send_responde(client_info->ssl, REQ_CREATE_CHAT, ERR_CHAT_EXIST);
        return;
    }

    sqlite3_reset(stmt);

    char *request = sqlite3_mprintf("INSERT INTO chats (name, type, date) VALUES('%s', '%d', '%d')", chat_name, type, date);
    db_execute_request(request);
    sqlite3_free(request);
 
    request = sqlite3_mprintf("SELECT id FROM chats WHERE name = '%s'", chat_name);

    sqlite3_prepare_v2(db, request, -1, &stmt, NULL);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        chat_id = sqlite3_column_int64(stmt, 0);
    }

    sqlite3_free(request);
    sqlite3_reset(stmt);
    sqlite3_close(db);

    request = sqlite3_mprintf("INSERT INTO members (chat_id, user_id, privilege) VALUES('%d', '%d', '%d')", 
            chat_id, client_info->user->id, USERTYPE_ADMIN);
    db_execute_request(request);
    
    sqlite3_free(request);

}

// static int get_chat_id(sqlite3 *db, const char *chat_name) {
//     sqlite3_stmt *stmt;
//     sqlite3_prepare_v2(db, "SELECT id FROM chats WHERE name = ?", -1, &stmt, NULL);
//     sqlite3_bind_text(stmt, 1, chat_name, -1, NULL);
//     int chat_id = -1;
//     if (sqlite3_step(stmt) == SQLITE_ROW) {
//         chat_id = sqlite3_column_int64(stmt, 0);
//     }
//     sqlite3_reset(stmt);
//     return chat_id;
// }

// static void insert_chat(sqlite3 *db, const char *chat_name, int type, int date) {
//     char *request = sqlite3_mprintf("INSERT INTO chats (name, type, date) VALUES('%s', '%d', '%d')", chat_name, type, date);
//     db_execute_request(request);
//     sqlite3_free(request);
// }

// static void insert_member(sqlite3 *db, int chat_id, int user_id, int privilege) {
//     char *request = sqlite3_mprintf("INSERT INTO members (chat_id, user_id, privilege) VALUES('%d', '%d', '%d')", 
//             chat_id, user_id, privilege);
//     db_execute_request(request);
//     sqlite3_free(request);
// }

// void create_chat(cJSON *json, t_client_info *client_info) {
//     db_init();
//     sqlite3 *db = db_open();

//     char *chat_name = cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring;
//     int type = cJSON_GetObjectItem(json, "chat_type")->valueint;
//     int date = cJSON_GetObjectItem(json, "time")->valueint;

//     int chat_id = get_chat_id(db, chat_name);
//     if (chat_id != -1) {
//         send_responde(client_info->ssl, REQ_CREATE_CHAT, ERR_CHAT_EXIST);
//         sqlite3_close(db);
//         return;
//     }

//     insert_chat(db, chat_name, type, date);
//     chat_id = get_chat_id(db, chat_name);
//     insert_member(db, chat_id, client_info->user->id, USERTYPE_ADMIN);

//     sqlite3_close(db);
// }

