#include "../../inc/server.h"

static int check_signup_errors(char *username)
{
    if (db_check_user_exists(username))
        return ERR_USER_EXISTS;

    return 0;
}

void user_signup(cJSON *json, t_client_info *client_info)
{
    char *username;
    unsigned char *password;
    int error_type;
    char *query;

    db_init();

    username = cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring;
    password = (unsigned char *)cJSON_GetObjectItemCaseSensitive(json, "password")->valuestring;

    error_type = check_signup_errors(username);
    if (error_type != 0)
    {
        send_responde(client_info->ssl, REQ_USER_SIGNUP, error_type);
        return;
    }

    query = sqlite3_mprintf("INSERT INTO users (username, password) VALUES('%s', '%s'); ",
                            username, password);
    db_execute_query(query);

    send_responde(client_info->ssl, REQ_USER_SIGNUP, ERR_SUCCESS);

    sqlite3_free(query);
}
