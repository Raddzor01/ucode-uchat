#include "../../inc/client.h"

int send_logout_to_server()
{
    account->is_busy = true;

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_LOGOUT);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);

    mx_strdel(&account->username);
    chat_clear_list(&account->chats);

    account->is_busy = false;

    return 0;
}