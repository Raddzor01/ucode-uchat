#include "../../inc/client.h"

int get_last_msg_id_from_server(int chat_id)
{
    int msg_id = 0;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_LAST_MSG_ID);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

#ifdef MACOS_VER
    sem_wait(account->semaphore);
#else
    sem_wait(&account->semaphore);
#endif
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
#ifdef MACOS_VER
    sem_post(account->semaphore);
#else
    sem_post(&account->semaphore);
#endif
    json = cJSON_Parse(json_str);

    int error_code = cJSON_GetObjectItem(json, "error_code")->valueint;
    if (error_code != ERR_SUCCESS)
    {
        printf("Error getting last message, error code - %d\n", error_code);
        if (error_code == ERR_CHAT_NONEXIST || error_code == ERR_USER_NOT_IN_CHAT)
            return CHAT_DIDNT_EXISTS;
        return CHAT_ERROR;
    }

    msg_id = cJSON_GetObjectItem(json, "message_id")->valueint;

    mx_strdel(&json_str);
    cJSON_Delete(json);

    return msg_id;
}
