#include "../../inc/server.h"

t_user_info *get_user_info(sqlite3 *db, char *username);
void log_successful_login(int id, char *username);
void login_user(t_client_info *client_info, t_user_info *user_info);
void send_login_response(SSL *ssl, t_user_info *user_info);

void user_login(cJSON *json, t_client_info *client_info)
{
    sqlite3 *db;
    char *username;
    unsigned char *password;
    t_user_info *user_info;

    db_init();

    username = cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring;
    password = (unsigned char *)cJSON_GetObjectItemCaseSensitive(json, "password")->valuestring;

    db = db_open();

    user_info = get_user_info(db, username);
    if (user_info == NULL)
    {
        send_responde(client_info->ssl, REQ_USER_LOGIN, ERR_USER_EXISTS);

        sqlite3_close(db);

        return;
    }

    if (memcmp(user_info->password, password, SHA256_DIGEST_LENGTH) != 0)
    {
        send_responde(client_info->ssl, REQ_USER_LOGIN, ERR_INVALID_PASSWORD);

        sqlite3_close(db);

        return;
    }

    login_user(client_info, user_info);
    send_login_response(client_info->ssl, user_info);
    log_successful_login(client_info->user->id, client_info->user->username);
}

t_user_info *get_user_info(sqlite3 *db, char *username)
{
    sqlite3_stmt *stmt;
    t_user_info *user_info = NULL;

    // sqlite3_prepare_v2(db, "SELECT users.id, users.username, users.password, users.image_id, files.filename, files.file_path, files.file_type FROM users "
    //                         "INNER JOIN files ON files.id = users.image_id "
    //                         "WHERE users.username = ?; ", -1, &stmt, NULL);
    // sqlite3_bind_text(stmt, 1, username, -1, NULL);

    sqlite3_prepare_v2(db, "SELECT users.id, users.username, users.password, users.image_id, files.filename FROM users "
                            "INNER JOIN files ON files.id = users.image_id "
                            "WHERE users.username = ?; ", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, -1, NULL);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        user_info = (t_user_info *)malloc(sizeof(t_user_info));
        user_info->id = sqlite3_column_int64(stmt, 0);
        user_info->username = mx_strdup((const char *)sqlite3_column_text(stmt, 1));
        user_info->password = (unsigned char *)mx_strdup((const char *)sqlite3_column_text(stmt, 2));
        user_info->image_id = sqlite3_column_int(stmt, 3);
        user_info->file_path = mx_strdup((const char *)sqlite3_column_text(stmt, 4));
        
    } else {
        mx_logs((char *)sqlite3_errmsg(db), LOG_ERROR);
    }

    sqlite3_finalize(stmt);
    return user_info;
}

void send_login_response(SSL *ssl, t_user_info *user_info)
{
    cJSON *json;
    char *json_str;

    json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_USER_LOGIN);
    cJSON_AddNumberToObject(json, "error_code", ERR_SUCCESS);
    cJSON_AddStringToObject(json, "username", user_info->username);
    cJSON_AddNumberToObject(json, "id", user_info->id);
    cJSON_AddNumberToObject(json, "image_id", user_info->image_id);
    cJSON_AddStringToObject(json, "password", (char *)user_info->password);
    cJSON_AddStringToObject(json, "filename", user_info->file_path);

    json_str = cJSON_PrintUnformatted(json);

    SSL_write(ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);
}

void login_user(t_client_info *client_info, t_user_info *user_info)
{
    client_info->user = user_info;
    add_client(client_info);
}

void log_successful_login(int id, char *username)
{
    char result_to_log[MAX_BUF_SIZE];
    sprintf(result_to_log, "Logged in id: %d, name: %s", id, username);
    mx_logs(result_to_log, LOG_INFO);
}
