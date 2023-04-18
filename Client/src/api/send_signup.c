#include "../../inc/client.h"

int check_account_exists()
{
    account->is_busy = true;

    char *json_str = read_from_server();
    pthread_mutex_unlock(&account->mutex);
    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        mx_printerr("Error getting user information for sign up\ncJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
    // t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

    // if (type == REQ_USER_SIGNUP) {
    // 	mx_printerr("Error in type\n");
    // 	return NULL;
    // }

    cJSON_Delete(json);
    free(json_str);

    account->is_busy = false;

    return error;
}

int send_sign_up_to_server(const char *username, const char *password)
{
    account->is_busy = true;
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_SIGNUP);
    cJSON_AddStringToObject(json, "username", username);
    cJSON_AddStringToObject(json, "password", password);
    char *json_str = cJSON_PrintUnformatted(json);

    pthread_mutex_lock(&account->mutex);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    account->is_busy = false;

    return check_account_exists();
}