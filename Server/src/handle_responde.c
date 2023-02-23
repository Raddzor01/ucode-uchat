#include "../inc/server.h"

void send_responde(SSL *ssl, t_req_type req_type, t_error_type err_code)
{

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "error_code", err_code);
    cJSON_AddNumberToObject(json, "type", req_type);
    char *response = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);

    SSL_write(ssl, response, mx_strlen(response));
    free(response);
}
