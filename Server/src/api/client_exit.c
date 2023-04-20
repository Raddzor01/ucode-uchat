#include "../../inc/server.h"

void client_exit(cJSON *__attribute__((unused)) json, t_client_info *client_info)
{
    SSL_free(client_info->ssl);
    close(client_info->socket_info);
    free(client_info);
    client_info = NULL;
    mx_logs("Client disconnected\n", LOG_INFO);
}
