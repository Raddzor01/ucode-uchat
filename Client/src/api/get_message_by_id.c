#include "../../inc/client.h"

t_msg *get_msg_by_id_from_server(int msg_id, int chat_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_MSG_BY_ID);
    cJSON_AddNumberToObject(json, "message_id", msg_id);
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

    if (cJSON_GetObjectItem(json, "error_code")->valueint != ERR_SUCCESS)
        return NULL;

    cJSON *message_json = cJSON_GetObjectItemCaseSensitive(json, "message");

    if(message_json == NULL)
        return NULL;

    t_msg *new_node = msg_prepare_node(msg_id,
                                    cJSON_GetObjectItemCaseSensitive(message_json, "message")->valuestring,
                                    cJSON_GetObjectItemCaseSensitive(message_json, "time")->valueint,
                                    cJSON_GetObjectItem(message_json, "user_id")->valueint,
                                    cJSON_GetObjectItemCaseSensitive(message_json, "username")->valuestring);
    new_node->image_id = cJSON_GetObjectItem(message_json, "image_id")->valueint;
    
    return new_node;
}
