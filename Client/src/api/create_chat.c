#include "../../inc/client.h"

int create_chat_in_server(const char *chat_name, int chat_type)
{
    account->is_busy = true;
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_CREATE_CHAT);
    cJSON_AddStringToObject(json, "name", chat_name);
    cJSON_AddNumberToObject(json, "chat_type", chat_type);
    cJSON_AddNumberToObject(json, "time", time(NULL));
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    account->is_busy = false;

    return 0;
}
