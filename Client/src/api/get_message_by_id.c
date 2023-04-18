#include "../../inc/client.h"

t_msg *get_msg_by_id_from_server(int msg_id, int chat_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_MSG_BY_ID);
    cJSON_AddNumberToObject(json, "message_id", msg_id);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    pthread_mutex_lock(&account->mutex);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
    pthread_mutex_unlock(&account->mutex);
    json = cJSON_Parse(json_str);

    if (cJSON_GetObjectItem(json, "error_code")->valueint != ERR_SUCCESS)
        return NULL;

    cJSON *message_json = cJSON_GetObjectItemCaseSensitive(json, "message");

    if(message_json == NULL)
        return NULL;

    t_msg *new_node = (t_msg *)malloc(sizeof(t_msg));
    new_node->text = mx_strdup(cJSON_GetObjectItemCaseSensitive(message_json, "message")->valuestring);
    new_node->msg_id = cJSON_GetObjectItem(message_json, "message_id")->valueint;
    new_node->user_id = cJSON_GetObjectItem(message_json, "user_id")->valueint;
    new_node->username = mx_strdup(cJSON_GetObjectItemCaseSensitive(message_json, "username")->valuestring);
    new_node->time = cJSON_GetObjectItemCaseSensitive(message_json, "time")->valueint;
    new_node->next = NULL;

    return new_node;
}
