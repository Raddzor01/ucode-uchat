#include "../../inc/server.h"

void get_last_msg(cJSON *json, t_client_info *client_info)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;
    int chat_id = 0;
    int msg_id = 0;

    chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

    if(!db_check_chat_exists(chat_id))
    {
        send_responde(client_info->ssl, REQ_GET_LAST_MSG, ERR_CHAT_NONEXIST);
        return;
    }

    db = db_open();
    query = sqlite3_mprintf("SELECT id FROM messages WHERE chat_id = %d ORDER BY id DESC LIMIT 1;",
                            chat_id);
    stmt = db_execute_query_and_return_stmt(query, db);

    msg_id = sqlite3_column_int(stmt, 0);

    cJSON *response = cJSON_CreateObject();
    cJSON_AddNumberToObject(response, "message_id", msg_id);
    cJSON_AddNumberToObject(response, "error_code", ERR_SUCCESS);
    char *json_str = cJSON_PrintUnformatted(response);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    sqlite3_finalize(stmt);
    sqlite3_free(query);
    sqlite3_close(db);
}
