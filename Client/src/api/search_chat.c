#include "../../inc/client.h"

t_chat *find_chats_from_server(const char *search_pattern)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_SEARCH_CHATS);
    cJSON_AddStringToObject(json, "search_pattern", search_pattern);
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

    cJSON *json_2 = cJSON_GetObjectItemCaseSensitive(json, "chats");
    t_chat *chats = NULL;
    cJSON *temp_json = NULL;
    pthread_mutex_lock(&account->mutex);
    for (int i = 0; i < cJSON_GetArraySize(json_2); i++)
    {
        temp_json = cJSON_GetArrayItem(json_2, i);

        int chat_id = cJSON_GetObjectItem(temp_json, "chat_id")->valueint;
        int image_id = cJSON_GetObjectItem(temp_json, "image_id")->valueint;
        t_chat *new_node = chat_prepare_node(chat_id, cJSON_GetObjectItemCaseSensitive(temp_json, "chat_name")->valuestring, image_id, PRIV_USER, CHAT_NORMAL);
        new_node->image_path = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "filename")->valuestring);
        new_node->messages = get_chat_messages_from_server(chat_id);
        new_node->users = get_chat_users_from_server(chat_id);
        new_node->users_count = 0;
        while (new_node->users[new_node->users_count])
            new_node->users_count++;
        chat_push_back(&chats, new_node);
    }
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return chats;
}
