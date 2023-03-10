#include "../../inc/server.h"

sqlite3_stmt *get_chats_stmt(sqlite3 *db, int64_t user_id);
cJSON *get_chats_json(sqlite3 *db, int user_id);
cJSON *get_chat_json(sqlite3_stmt *stmt);
void send_chats_json_to_client(cJSON *json, t_client_info *client_info);

void get_chats(cJSON *chat_info, t_client_info *client_info) {

    sqlite3 *db = db_open();

    cJSON *json = get_chats_json(db, client_info->user->id);
    send_chats_json_to_client(json, client_info);

    sqlite3_close(db);

}

cJSON *get_chats_json(sqlite3 *db, int user_id) {

    sqlite3_stmt *stmt = get_chats_stmt(db, user_id);

    cJSON *chats_array = cJSON_CreateArray();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON_AddItemToArray(chats_array, get_chat_json(stmt));
    }

    cJSON *json = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(json, "chats", chats_array);
    cJSON_AddNumberToObject(json, "type", REQ_GET_CHATS);
    cJSON_AddNumberToObject(json, "error_code", ERR_SUCCESS);

    sqlite3_finalize(stmt);

    return json;

}

sqlite3_stmt *get_chats_stmt(sqlite3 *db, int64_t user_id) {

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT chats.id, chats.name, chats.type, members.privilege FROM chats "
                           "INNER JOIN members ON members.chat_id = chats.id "
                           "WHERE chats.id IN (SELECT chat_id FROM members WHERE user_id = ?) AND members.user_id = ? "
                           "ORDER BY chats.date DESC ",
                       -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, user_id);
    sqlite3_bind_int64(stmt, 2, user_id);

    return stmt;

}

cJSON *get_chat_json(sqlite3_stmt *stmt) {

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "chat_id", sqlite3_column_int64(stmt, 0));
    cJSON_AddStringToObject(json, "chat_name", (const char *)sqlite3_column_text(stmt, 1));
    cJSON_AddNumberToObject(json, "chat_type", sqlite3_column_int64(stmt, 2));
    cJSON_AddNumberToObject(json, "chat_permissions", sqlite3_column_int64(stmt, 3));

    return json;
    
}

void send_chats_json_to_client(cJSON *json, t_client_info *client_info) {

    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    free(json_str);
    cJSON_Delete(json);

}
