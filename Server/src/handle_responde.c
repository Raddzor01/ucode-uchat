#include "../inc/server.h"

void handle_responde(cJSON *json, t_client_info *client_info)
{
    // client_info->user->password = NULL;
    mx_logs(cJSON_Print(json), INFO_LOG);
    mx_logs("Check success", INFO_LOG);
}

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
