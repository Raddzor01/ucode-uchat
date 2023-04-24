#include "../../inc/client.h"

int edit_msg_in_server(int msg_id, const char *new_text)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_EDIT_MESSAGE);
    cJSON_AddNumberToObject(json, "id", msg_id);
    cJSON_AddNumberToObject(json, "chat_id", account->current_chat->id);
    cJSON_AddStringToObject(json, "text", new_text);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    t_msg *edited_msg = msg_get_by_id(account->current_chat->messages, msg_id);
    mx_strdel(&edited_msg->text);

    pthread_mutex_lock(&account->mutex);
    edited_msg->text = mx_strdup(new_text);
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    free(json_str);

    return 0;
}
