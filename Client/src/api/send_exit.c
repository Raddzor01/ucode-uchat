#include "../../inc/client.h"

int send_exit_to_server()
{
    pthread_cancel(account->server_update_thread);
    if (account->username != NULL)
        send_logout_to_server();

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_CLIENT_EXIT);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return 0;
}
