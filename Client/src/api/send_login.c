#include "../../inc/client.h"

bool check_account_from_server()
{
    char *json_str = read_from_server();

    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        mx_strdel(&json_str);
        return ERR_JSON;
    }

    int error = cJSON_GetObjectItem(json, "error_code")->valueint;

    // add error checks
    if (error != 0)
    {
        cJSON_Delete(json);
        mx_strdel(&json_str);
        return error;
    }

    pthread_mutex_lock(&account->mutex);
    account->id = cJSON_GetObjectItem(json, "id")->valueint;
    account->username = mx_strdup(cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring);
    account->password = (unsigned char *)mx_strdup(cJSON_GetObjectItemCaseSensitive(json, "password")->valuestring);
    account->image_id = cJSON_GetObjectItem(json, "image_id")->valueint;
    account->image_path =  mx_strjoin(DATA_DIR, cJSON_GetObjectItemCaseSensitive(json, "filename")->valuestring);
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return ERR_SUCCESS;
}

int send_login_to_server(const char *username, const unsigned char *password)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_LOGIN);
    cJSON_AddStringToObject(json, "username", username);
    cJSON_AddStringToObject(json, "password", (char *)password);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return check_account_from_server();
}
