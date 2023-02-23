#include "../../inc/server.h"

int check_signup_errors(cJSON *username, cJSON *password)
{

    if (!cJSON_IsString(username) || !cJSON_IsString(password))
        return ERR_JSON;

    if (db_check_user_exists(username->valuestring))
        return ERR_USER_EXISTS;

    return 0;
}

void user_signup(cJSON *json, t_client_info *client_info)
{

    db_init();

    cJSON *username = cJSON_GetObjectItemCaseSensitive(json, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(json, "password");

    int error_type = 0;
    if ((error_type = check_signup_errors(username, password)) != 0)
    {
        send_responde(client_info->ssl, REQ_USER_SIGNUP, error_type);
        return;
    }

    char *request = sqlite3_mprintf("INSERT INTO users (username, password) VALUES('%s', '%s')", username->valuestring, password->valuestring);
    db_execute_request(request);
    sqlite3_free(request);

    send_responde(client_info->ssl, REQ_USER_SIGNUP, ERR_SUCCESS);
    
}
