#include "../../inc/server.h"

cJSON *get_chat_json(sqlite3_stmt *stmt) {

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "chat_id", sqlite3_column_int64(stmt, 0));
    cJSON_AddStringToObject(json, "chat_name", (const char *)sqlite3_column_text(stmt, 1));

    return json;
    
}

cJSON *get_user_json(sqlite3_stmt *stmt) {

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "user_id", sqlite3_column_int64(stmt, 2));
    cJSON_AddStringToObject(json, "username", (const char *)sqlite3_column_text(stmt, 3));

    return json;
    
}


void search_chats(cJSON *client_json, t_client_info *client_info) {

    sqlite3 *db = db_open();
    sqlite3_stmt *stmt;

    char *search_pattern = cJSON_GetObjectItemCaseSensitive(client_json, "search_pattern")->valuestring;
    
    char* search_str = mx_strnew(mx_strlen(search_pattern) + 3);
    sprintf(search_str, "%%%s%%", search_pattern);

    cJSON *users_json = cJSON_CreateArray();
    cJSON *chats_json = cJSON_CreateArray();

    const char *sql_query = "SELECT c.id, c.name, u.id AS user_id, u.username "
                            "FROM chats c "
                            "INNER JOIN members m1 ON c.id = m1.chat_id "
                            "INNER JOIN users u ON m1.user_id = u.id "
                            "LEFT JOIN members m2 ON c.id = m2.chat_id AND m2.user_id = ? "
                            "WHERE u.username LIKE ? "
                            "AND (m2.id IS NULL OR c.type != 1) "
                            "ORDER BY c.date DESC; ";

    int result = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    
    // if (result != SQLITE_OK) {
    //     // проверку ошибки огромной хуйни сверху надо реализовать
    // }

    sqlite3_bind_int64(stmt, 1, client_info->user->id);
    sqlite3_bind_text(stmt, 2, search_str, -1, NULL);

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        cJSON_AddItemToArray(chats_json, get_chat_json(stmt));
        cJSON_AddItemToArray(users_json, get_user_json(stmt));
    
    }

    cJSON *json = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(json, "chats", chats_json);
    cJSON_AddItemReferenceToObject(json, "users", users_json);
    cJSON_AddNumberToObject(json, "type", REQ_SEARCH_CHATS);
    cJSON_AddNumberToObject(json, "error_code", ERR_SUCCESS);

    char* json_str = cJSON_PrintUnformatted(json);
    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);
}    