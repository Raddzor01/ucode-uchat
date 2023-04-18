#include "../../inc/client.h"

t_msg *get_chat_messages_from_server(int chat_id)
{
    account->is_busy = true;

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_GET_CHAT_MESSAGES);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    cJSON *messages_arr = cJSON_GetObjectItemCaseSensitive(json, "messages");
    t_msg *msgs = NULL;

    cJSON *temp_json = NULL;
    for (int i = 0; i < cJSON_GetArraySize(messages_arr); i++)
    {
        temp_json = cJSON_GetArrayItem(messages_arr, i);

        t_msg *temp = (t_msg *)malloc(sizeof(t_msg));
        temp->text = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "message")->valuestring);
        temp->msg_id = cJSON_GetObjectItem(temp_json, "message_id")->valueint;
        temp->user_id = cJSON_GetObjectItem(temp_json, "user_id")->valueint;
        temp->username = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "username")->valuestring);
        temp->time = cJSON_GetObjectItemCaseSensitive(temp_json, "date")->valueint;
        temp->next = NULL;

        msg_push_back(&msgs, temp);
    }

    cJSON_Delete(json);
    mx_strdel(&json_str);

    account->is_busy = false;

    return msgs;
}
