#include "../../inc/client.h"

int send_logout_to_server()
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_LOGOUT);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    username_display = TRUE;

    mx_strdel(&account->username);
    free(account->password);
    account->password = NULL;
    mx_strdel(&account->image_path);
    pthread_cancel(account->server_update_thread);
    pthread_cancel(account->server_online_thread);
    chat_clear_list(&account->chats);
    
    return 0;
}
