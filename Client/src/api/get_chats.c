#include "../../inc/client.h"

int get_user_chats()
{
    account->is_busy = true;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_CHATS);

    char *json_str = cJSON_PrintUnformatted(json);
    
    pthread_mutex_lock(&account->mutex);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    cJSON *chats_array = cJSON_Parse(json_str);
    json = cJSON_GetObjectItemCaseSensitive(chats_array, "chats");

    cJSON *temp_json = NULL;
    for (int i = 0; i < cJSON_GetArraySize(json); i++)
    {
        temp_json = cJSON_GetArrayItem(json, i);

        t_chat *new_node = (t_chat *)malloc(sizeof(t_chat));
        new_node->name = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "chat_name")->valuestring);
        new_node->id = cJSON_GetObjectItem(temp_json, "chat_id")->valueint;
        new_node->messages = get_chat_messages_from_server(new_node->id);
        new_node->next = NULL;
        chat_push_back(&account->chats, new_node);
    }
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    mx_strdel(&json_str);

    account->is_busy = false;

    return 0;
}