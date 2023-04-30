#include "../../inc/server.h"

void get_chat_users(cJSON *json, t_client_info *client_info)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    cJSON *responde = NULL;
    char *query = NULL;

    int chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;

    db = db_open();
    query = sqlite3_mprintf("SELECT users.id, users.username, users.image_id, members.privilege "
                            "FROM users "
                            "JOIN members ON users.id = members.user_id "
                            "JOIN chats ON members.chat_id = chats.id "
                            "WHERE chats.id = %d; ",
                            chat_id);
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    cJSON *users = cJSON_CreateArray();
    cJSON *temp = NULL;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        temp = cJSON_CreateObject();
        cJSON_AddNumberToObject(temp, "id", sqlite3_column_int(stmt, 0));
        cJSON_AddStringToObject(temp, "username", (const char *)sqlite3_column_text(stmt, 1));
        cJSON_AddNumberToObject(temp, "image_id", sqlite3_column_int(stmt, 2));
        cJSON_AddNumberToObject(temp, "privilege", sqlite3_column_int(stmt, 3));

        cJSON_AddItemToArray(users, temp);
    }

    responde = cJSON_CreateObject();
    cJSON_AddNumberToObject(responde, "type", REQ_GET_CHAT_USERS);
    cJSON_AddNumberToObject(responde, "error_code", ERR_SUCCESS);
    cJSON_AddItemReferenceToObject(responde, "users", users);
    char *json_str = cJSON_PrintUnformatted(responde);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    sqlite3_free(query);
    mx_strdel(&json_str);
    cJSON_Delete(responde);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
