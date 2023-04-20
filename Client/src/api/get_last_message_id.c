#include "../../inc/client.h"

int get_last_msg_id_from_server(int chat_id)
{
    int msg_id = 0;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_LAST_MSG_ID);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    sem_wait(&account->semaphore);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
    sem_post(&account->semaphore);
    json = cJSON_Parse(json_str);

    int error_code = cJSON_GetObjectItem(json, "error_code")->valueint;
    if (error_code != 0)
    {
        printf("Error getting last message, error code - %d\n", error_code);
        return -1;
    }

    msg_id = cJSON_GetObjectItem(json, "message_id")->valueint;

    mx_strdel(&json_str);
    cJSON_Delete(json);

    return msg_id;
}
