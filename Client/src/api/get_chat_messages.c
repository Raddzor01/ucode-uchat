#include "../../inc/client.h"

t_msg *get_chat_messages_from_server(int chat_id)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_GET_CHAT_MESSAGES);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

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

    cJSON *messages_arr = cJSON_GetObjectItemCaseSensitive(json, "messages");
    t_msg *msgs = NULL;

    cJSON *message_json = NULL;
    for (int i = 0; i < cJSON_GetArraySize(messages_arr); i++)
    {
        message_json = cJSON_GetArrayItem(messages_arr, i);

        t_msg *new_node = msg_prepare_node(cJSON_GetObjectItem(message_json, "message_id")->valueint,
                                           cJSON_GetObjectItemCaseSensitive(message_json, "message")->valuestring,
                                           cJSON_GetObjectItemCaseSensitive(message_json, "date")->valueint,
                                           cJSON_GetObjectItem(message_json, "user_id")->valueint,
                                           cJSON_GetObjectItemCaseSensitive(message_json, "username")->valuestring);
        new_node->image_id = cJSON_GetObjectItem(message_json, "image_id")->valueint;
        msg_push_back(&msgs, new_node);
    }

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return msgs;
}
