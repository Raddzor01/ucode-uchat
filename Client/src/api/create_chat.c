#include "../../inc/client.h"

int check_chat_id_from_server()
{
    char *json_str = read_from_server();
    pthread_mutex_unlock(&account->mutex);

    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        mx_printerr("Error getting new chat ID\ncJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

    cJSON_Delete(json);
    free(json_str);

    account->is_busy = false;

    return chat_id;
}

int create_chat_in_server(const char *chat_name, int chat_type)
{
    account->is_busy = true;

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_CREATE_CHAT);
    cJSON_AddStringToObject(json, "name", chat_name);
    cJSON_AddNumberToObject(json, "chat_type", chat_type);
    cJSON_AddNumberToObject(json, "time", time(NULL));
    char *json_str = cJSON_PrintUnformatted(json);

    pthread_mutex_lock(&account->mutex);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return check_chat_id_from_server();
}
