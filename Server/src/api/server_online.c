#include "../../inc/server.h"

void server_online(cJSON *json, t_client_info *client_info)
{
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
}
