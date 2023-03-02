#include "../../inc/server.h"

void user_login(cJSON *json, t_client_info *client_info) {

    db_init();
    sqlite3 *db = db_open();
    sqlite3_stmt *stmt;

    cJSON *username = cJSON_GetObjectItemCaseSensitive(json, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(json, "password");

    sqlite3_prepare_v2(db,  "SELECT users.id, users.username, users.password "
                            "FROM users WHERE username = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username->valuestring, -1, NULL);
    if(sqlite3_step(stmt) == SQLITE_ROW) {
        client_info->user =(t_user_info *) malloc(sizeof(t_user_info));
        client_info->user->id = sqlite3_column_int64(stmt, 0);
        client_info->user->username = mx_strdup((const char*)sqlite3_column_text(stmt, 1));
        client_info->user->password = mx_strdup((const char*)sqlite3_column_text(stmt, 2));
        add_client(client_info);
    }
    sqlite3_finalize(stmt);

    if(client_info->user == NULL) {
        send_responde(client_info->ssl, REQ_USER_LOGIN, ERR_USER_EXISTS);
        return;
    }
    if(mx_strcmp(find_client(client_info->user->id)->user->password, password->valuestring) != 0) {
        remove_client(client_info->user->id);
        send_responde(client_info->ssl, REQ_USER_LOGIN, ERR_INVALID_PASSWORD);
        return;
    }

    cJSON *json1 = cJSON_CreateObject();
    cJSON_AddStringToObject(json1, "username", client_info->user->username);
    cJSON_AddStringToObject(json1, "password", client_info->user->password);
    cJSON_AddNumberToObject(json1, "type", REQ_USER_LOGIN);
    cJSON_AddNumberToObject(json1, "id", client_info->user->id);
    cJSON_AddNumberToObject(json1, "error_code", ERR_SUCCESS);
    char* user_info = cJSON_PrintUnformatted(json1);
    mx_logs(user_info, INFO_LOG);
    cJSON_Delete(json1);
    SSL_write(client_info->ssl, user_info, mx_strlen(user_info));
    mx_strdel(&user_info);

    char result_to_log[MAX_BUF_SIZE];
    sprintf(result_to_log, "Logged in id: %d, name: %s", 
            client_info->user->id,
            client_info->user->username);
    mx_logs(result_to_log, INFO_LOG);

}
