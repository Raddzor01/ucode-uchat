#include "../../inc/client.h"

bool check_account_from_server()
{
    account->is_busy = true;
    char *json_str = read_from_server();
    pthread_mutex_unlock(&account->mutex);

    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        mx_printerr("Error getting user information for login\ncJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return 0;
    }

    t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
    // t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

    // add error checks
    if (error != 0)
    {
        mx_printerr("Error getting user information for login\nError in account cJSON\n");
        cJSON_Delete(json);
        free(json_str);
        return 0;
    }

    pthread_mutex_lock(&account->mutex);
    account->id = cJSON_GetObjectItem(json, "id")->valueint;
    account->username = mx_strdup(cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring);
    pthread_mutex_unlock(&account->mutex);
    // char *password = cJSON_GetObjectItem(json, "password")->valuestring;
    // printf("%d\t%s\n", account->id, account->username);
    cJSON_Delete(json);
    free(json_str);

    account->is_busy = false;

    return 1;
}

int send_login_to_server(const char *username, const char *password)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_LOGIN);
    cJSON_AddStringToObject(json, "username", username);
    cJSON_AddStringToObject(json, "password", password);
    char *json_str = cJSON_PrintUnformatted(json);

    pthread_mutex_lock(&account->mutex);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return check_account_from_server();
}
