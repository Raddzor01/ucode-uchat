#include "../../inc/client.h"

int send_logout_to_server()
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_LOGOUT);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);

    mx_strdel(&account->username);
    pthread_cancel(account->server_update_thread);
    chat_clear_list(&account->chats);
    pthread_create(&account->server_update_thread, NULL, server_update_thread, NULL);
    
    return 0;
}
