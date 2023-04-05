#include "../inc/server.h"

void send_responde(SSL *ssl, t_req_type req_type, t_error_type err_code)
{
    cJSON *json;
    char *json_str;

    json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "error_code", err_code);
    cJSON_AddNumberToObject(json, "type", req_type);
    json_str = cJSON_PrintUnformatted(json);

    SSL_write(ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);
}
