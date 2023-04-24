#include "../../inc/client.h"

int join_to_found_chat(int chat_id)
{
    cJSON *json = NULL;
    char *json_str = NULL;
    int error_code = 0;

    json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_JOIN_CHAT);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    json_str = cJSON_PrintUnformatted(json);

#ifdef MACOS_VER
    sem_wait(account->semaphore);
#else
    sem_wait(&account->semaphore);
#endif
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
#ifdef MACOS_VER
    sem_post(account->semaphore);
#else
    sem_post(&account->semaphore);
#endif
    json = cJSON_Parse(json_str);

    error_code = cJSON_GetObjectItem(json, "error_code")->valueint;
    
    cJSON_Delete(json);
    mx_strdel(&json_str);

    return error_code;
}

