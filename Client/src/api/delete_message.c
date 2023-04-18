#include "../../inc/client.h"

int delete_msg_in_server(int msg_id)
{

    account->is_busy = true;

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_DEL_MESSAGE);
    cJSON_AddNumberToObject(json, "chat_id", account->current_chat->id);
    cJSON_AddNumberToObject(json, "message_id", msg_id);

    char *json_str = cJSON_PrintUnformatted(json);
    pthread_mutex_lock(&account->mutex);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    pthread_mutex_unlock(&account->mutex);
    json = cJSON_Parse(json_str);

    if (cJSON_GetObjectItem(json, "error_code")->valueint != 0)
        printf("Error deleting message\n");

    pthread_mutex_lock(&account->mutex);
    msg_pop_by_message_id(&account->current_chat->messages, msg_id);
    pthread_mutex_unlock(&account->mutex);

    mx_strdel(&json_str);
    cJSON_Delete(json);

    account->is_busy = false;

    return 0;
}
