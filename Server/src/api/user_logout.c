#include "../../inc/server.h"

void log_logout(const int id, const char *username)
{
    char result_to_log[128];
    sprintf(result_to_log, "User logged out id: %d, name: %s", id, username);
    mx_logs(result_to_log, LOG_INFO);
}

void user_logout(cJSON *__attribute__((unused)) json, t_client_info *client_info)
{
    log_logout(client_info->user->id, client_info->user->username);
    remove_client(client_info->hash_id);
    free(client_info->user->password);
    client_info->user->password = NULL;
    mx_strdel(&client_info->user->username);
    free(client_info->user);
    client_info->user = NULL;
}
