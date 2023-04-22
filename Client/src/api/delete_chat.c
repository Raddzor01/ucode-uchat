#include "../../inc/client.h"

int delete_chat_in_server()
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_DEL_CHAT);
    cJSON_AddNumberToObject(json, "chat_id", account->current_chat->id);
    char *json_str = cJSON_PrintUnformatted(json);

    sem_wait(&account->semaphore);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    sem_post(&account->semaphore);
    json = cJSON_Parse(json_str);
    
    if (cJSON_GetObjectItem(json, "error_code")->valueint != 0)
        return 1;

    mx_strdel(&json_str);
    cJSON_Delete(json);

    return 0;
}
