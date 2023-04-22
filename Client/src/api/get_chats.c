#include "../../inc/client.h"

int get_user_chats()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_CHATS);
    char *json_str = cJSON_PrintUnformatted(json);
    
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

        int chat_id = cJSON_GetObjectItem(temp_json, "chat_id")->valueint;
        int image_id = cJSON_GetObjectItem(temp_json, "image_id")->valueint;
        t_chat *new_node = chat_prepare_node(chat_id, cJSON_GetObjectItemCaseSensitive(temp_json, "chat_name")->valuestring, image_id);
        new_node->messages = get_chat_messages_from_server(chat_id);
        chat_push_back(&account->chats, new_node);
    }

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return 0;
}
