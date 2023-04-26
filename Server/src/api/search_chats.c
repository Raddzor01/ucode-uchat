#include "../../inc/server.h"

cJSON *get_chat_json(sqlite3_stmt *stmt)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "chat_id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(json, "chat_name", (const char *)sqlite3_column_text(stmt, 1));
    cJSON_AddNumberToObject(json, "image_id", sqlite3_column_int(stmt, 2));
    cJSON_AddStringToObject(json, "filename", (const char *)sqlite3_column_text(stmt, 3));

    return json;
}

cJSON *get_user_json(sqlite3_stmt *stmt)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "user_id", sqlite3_column_int64(stmt, 2));
    cJSON_AddStringToObject(json, "username", (const char *)sqlite3_column_text(stmt, 3));

    return json;
}

void search_chats(cJSON *client_json, t_client_info *client_info)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;

    char *search_pattern = cJSON_GetObjectItemCaseSensitive(client_json, "search_pattern")->valuestring;

    char *search_str = mx_strnew(mx_strlen(search_pattern) + 3);
    sprintf(search_str, "%%%s%%", search_pattern);

    cJSON *chats_json = cJSON_CreateArray();

    // const char *query = "SELECT c.id, c.name, u.id AS user_id, u.username "
    //                         "FROM chats c "
    //                         "INNER JOIN members m1 ON c.id = m1.chat_id "
    //                         "INNER JOIN users u ON m1.user_id = u.id "
    //                         "LEFT JOIN members m2 ON c.id = m2.chat_id AND m2.user_id = ? "
    //                         "WHERE u.username LIKE ? "
    //                         "AND (m2.id IS NULL OR c.type != 1) "
    //                         "ORDER BY c.date DESC; ";

    query = sqlite3_mprintf("SELECT chats.id, chats.name, chats.image_id, files.filename FROM chats "
                            "INNER JOIN files ON files.id = chats.image_id "
                            "WHERE name LIKE '%s' "
                            "AND chats.id NOT IN (SELECT chat_id FROM members WHERE user_id = %d) "
                            "AND chats.type NOT IN (%d, %d) "
                            "ORDER BY chats.date DESC; ",
                            search_str, client_info->user->id, CHAT_PRIVATE, CHAT_SAVED);
    
    db = db_open();
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cJSON_AddItemToArray(chats_json, get_chat_json(stmt));
        // cJSON_AddItemToArray(users_json, get_user_json(stmt));
        mx_logs((char *)sqlite3_errmsg(db), LOG_ERROR);
    }
    mx_logs((char *)sqlite3_errmsg(db), LOG_ERROR);

    cJSON *json = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(json, "chats", chats_json);
    // cJSON_AddItemReferenceToObject(json, "users", users_json);
    cJSON_AddNumberToObject(json, "type", REQ_SEARCH_CHATS);
    cJSON_AddNumberToObject(json, "error_code", ERR_SUCCESS);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    mx_strdel(&search_str);
    cJSON_Delete(json);
    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
