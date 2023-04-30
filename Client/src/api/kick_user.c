#include "../../inc/client.h"

int kick_user_in_server(int chat_id, int user_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_KICK_USER);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    cJSON_AddNumberToObject(json, "user_id", user_id);
    char *json_str = cJSON_PrintUnformatted(json);

    sem_wait(&account->semaphore);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    sem_post(&account->semaphore);
    json = cJSON_Parse(json_str);

    int error_code = cJSON_GetObjectItem(json, "error_code")->valueint;
    
    return error_code;
}
