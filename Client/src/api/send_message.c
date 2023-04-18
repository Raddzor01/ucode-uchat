#include "../../inc/client.h"

time_t send_message_to_server(const char *message)
{

    account->is_busy = true;

    time_t current_time = time(NULL);

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_SEND_MSG);
    cJSON_AddNumberToObject(json, "user_id", account->id);
    cJSON_AddNumberToObject(json, "chat_id", account->current_chat->id);
    cJSON_AddStringToObject(json, "message", message);
    cJSON_AddStringToObject(json, "username", account->username);
    cJSON_AddNumberToObject(json, "time", current_time);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    account->is_busy = false;

    return current_time;
}

int get_msg_id()
{
    account->is_busy = true;

    char *json_str = read_from_server();
    cJSON *json = cJSON_Parse(json_str);
    int msg_id;

    if (json == NULL)
    {
        mx_printerr("Error receiving id message\ncJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    msg_id = cJSON_GetObjectItem(json, "message_id")->valueint;

    cJSON_Delete(json);
    free(json_str);

    account->is_busy = false;

    return msg_id;
}

