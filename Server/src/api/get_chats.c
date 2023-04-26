#include "../../inc/server.h"

sqlite3_stmt *get_chats_stmt(sqlite3 *db, int user_id);
cJSON *get_chats_json(sqlite3 *db, int user_id);
static cJSON *get_chat_json(sqlite3_stmt *stmt);
void send_chats_json_to_client(cJSON *json, t_client_info *client_info);

void get_chats(cJSON *__attribute__((unused)) json, t_client_info *client_info)
{
    sqlite3 *db;
    cJSON *clinet_json;

    db = db_open();
    clinet_json = get_chats_json(db, client_info->user->id);
    send_chats_json_to_client(clinet_json, client_info);

    sqlite3_close(db);
}

cJSON *get_chats_json(sqlite3 *db, int user_id)
{
    sqlite3_stmt *stmt;
    cJSON *chats_array;
    cJSON *json;

    stmt = get_chats_stmt(db, user_id);

    chats_array = cJSON_CreateArray();
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cJSON_AddItemToArray(chats_array, get_chat_json(stmt));
    }

    json = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(json, "chats", chats_array);
    cJSON_AddNumberToObject(json, "type", REQ_GET_CHATS);
    cJSON_AddNumberToObject(json, "error_code", ERR_SUCCESS);

    sqlite3_finalize(stmt);

    return json;
}

sqlite3_stmt *get_chats_stmt(sqlite3 *db, int user_id)
{
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT chats.id, chats.name, chats.type, chats.image_id, members.privilege, files.filename FROM chats "
                           "INNER JOIN members ON members.chat_id = chats.id "
                           "INNER JOIN files ON files.id = chats.image_id "
                           "WHERE chats.id IN (SELECT chat_id FROM members WHERE user_id = ?) AND members.user_id = ? "
                           "ORDER BY chats.date DESC; ",
                       -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, user_id);

    return stmt;
}

static cJSON *get_chat_json(sqlite3_stmt *stmt)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "chat_id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(json, "chat_name", (const char *)sqlite3_column_text(stmt, 1));
    cJSON_AddNumberToObject(json, "chat_type", sqlite3_column_int(stmt, 2));
    cJSON_AddNumberToObject(json, "image_id", sqlite3_column_int(stmt, 3));
    cJSON_AddNumberToObject(json, "user_privilege", sqlite3_column_int(stmt, 4));
    cJSON_AddStringToObject(json, "filename", (const char *)sqlite3_column_text(stmt, 5));

    return json;
}

void send_chats_json_to_client(cJSON *json, t_client_info *client_info)
{
    char *json_str;

    json_str = cJSON_PrintUnformatted(json);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);
}
