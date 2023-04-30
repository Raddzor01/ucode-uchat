#include "../../inc/client.h"

t_chat *get_user_chats()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_CHATS);
    char *json_str = cJSON_PrintUnformatted(json);
    
    sem_wait(&account->semaphore);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    sem_post(&account->semaphore);
    cJSON *chats_array = cJSON_Parse(json_str);
    json = cJSON_GetObjectItemCaseSensitive(chats_array, "chats");

    t_chat *chats = NULL;

    cJSON *temp_json = NULL;
    for (int i = 0; i < cJSON_GetArraySize(json); i++)
    {
        temp_json = cJSON_GetArrayItem(json, i);

        int chat_type = cJSON_GetObjectItem(temp_json, "chat_type")->valueint;
        int chat_id = cJSON_GetObjectItem(temp_json, "chat_id")->valueint;
        int image_id = cJSON_GetObjectItem(temp_json, "image_id")->valueint;
        int user_privilege = cJSON_GetObjectItem(temp_json, "user_privilege")->valueint;
        t_chat *new_node = chat_prepare_node(chat_id, chat_type == CHAT_SAVED ? "Saved messages" : cJSON_GetObjectItemCaseSensitive(temp_json, "chat_name")->valuestring, image_id, user_privilege, chat_type);
        new_node->messages = get_chat_messages_from_server(chat_id);
        new_node->users = get_chat_users_from_server(chat_id);
        new_node->users_count = 0;
        while (new_node->users[new_node->users_count] != NULL)
            new_node->users_count++;
        new_node->image_path = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "filename")->valuestring);
        chat_push_back(&chats, new_node);
    }

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return chats;
}
