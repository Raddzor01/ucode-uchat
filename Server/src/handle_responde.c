#include "../inc/server.h"

void handle_responde(cJSON *json, t_client_info *client_info) {
    // client_info->user->password = NULL;
    mx_logs(cJSON_Print(json), INFO_LOG);
    mx_logs("Check success", INFO_LOG);
}
