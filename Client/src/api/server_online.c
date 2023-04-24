#include "../../inc/client.h"

bool check_server_online()
{
    char *responde = NULL;
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_SERVER_ONLINE);
    char *json_str = cJSON_PrintUnformatted(json);

#ifdef MACOS_VER
    sem_wait(account->semaphore);
#else
    sem_wait(&account->semaphore);
#endif
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    responde = read_from_server();
#ifdef MACOS_VER
    sem_post(account->semaphore);
#else
    sem_post(&account->semaphore);
#endif
    if (responde == NULL)
    {
        mx_strdel(&responde);
        mx_strdel(&json_str);
        cJSON_Delete(json);
        return true;
    }

    mx_strdel(&responde);
    mx_strdel(&json_str);
    cJSON_Delete(json);
    return false;
}
