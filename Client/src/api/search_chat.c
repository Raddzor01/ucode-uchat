#include "../../inc/client.h"

t_chat *find_chats_from_server(const char *search_pattern)
{

    account->is_busy = true;

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_SEARCH_CHATS);
    cJSON_AddStringToObject(json, "search_pattern", search_pattern);
    char *json_str = cJSON_PrintUnformatted(json);

    sem_wait(&account->semaphore);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    sem_post(&account->semaphore);

    json = cJSON_Parse(json_str);

    cJSON *json_2 = cJSON_GetObjectItemCaseSensitive(json, "chats");
    t_chat *chats = NULL;
    cJSON *temp_json = NULL;
    pthread_mutex_lock(&account->mutex);
    for (int i = 0; i < cJSON_GetArraySize(json_2); i++)
    {
        temp_json = cJSON_GetArrayItem(json_2, i);
                                
        int chat_id = cJSON_GetObjectItem(temp_json, "chat_id")->valueint;
        int image_id = cJSON_GetObjectItem(temp_json, "image_id")->valueint;
        t_chat *new_node = chat_prepare_node(chat_id, cJSON_GetObjectItemCaseSensitive(temp_json, "chat_name")->valuestring, image_id, PRIV_USER);
        new_node->messages = get_chat_messages_from_server(chat_id);
        chat_push_back(&chats, new_node);
    }
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    mx_strdel(&json_str);

    account->is_busy = false;

    return chats;
}
