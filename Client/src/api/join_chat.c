#include "../../inc/client.h"

int join_to_found_chat(int chat_id)
{
    account->is_busy = true;

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_JOIN_CHAT);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    account->is_busy = false;

    return 0;
}

