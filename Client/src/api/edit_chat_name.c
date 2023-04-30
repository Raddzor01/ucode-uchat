#include "../../inc/client.h"

int edit_chat_name_in_server(const char *new_name)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_EDIT_CHAT_NAME);
    cJSON_AddNumberToObject(json, "chat_id", account->current_chat->id);
    cJSON_AddStringToObject(json, "new_name", new_name);
    char *json_str = cJSON_PrintUnformatted(json);

    sem_wait(&account->semaphore);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    sem_post(&account->semaphore);
    json = cJSON_Parse(json_str);

    int error_code = cJSON_GetObjectItem(json, "error_code")->valueint;
    if (error_code != ERR_SUCCESS)
    {
        cJSON_Delete(json);
        mx_strdel(&json_str);
        return error_code;
    }

    pthread_mutex_lock(&account->mutex);
    mx_strdel(&account->current_chat->name);
    account->current_chat->name = mx_strdup(new_name);
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return ERR_SUCCESS;
}
