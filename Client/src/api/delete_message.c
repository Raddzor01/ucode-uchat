#include "../../inc/client.h"

int delete_msg_in_server(int msg_id)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_DEL_MESSAGE);
    cJSON_AddNumberToObject(json, "chat_id", account->current_chat->id);
    cJSON_AddNumberToObject(json, "message_id", msg_id);
    char *json_str = cJSON_PrintUnformatted(json);

#ifdef MACOS_VER
    sem_wait(account->semaphore);
#else
    sem_wait(&account->semaphore);
#endif
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
#ifdef MACOS_VER
    sem_post(account->semaphore);
#else
    sem_post(&account->semaphore);
#endif
    json = cJSON_Parse(json_str);

    if (cJSON_GetObjectItem(json, "error_code")->valueint != 0)
        pop_up_window("Error deleting message");

    mx_strdel(&json_str);
    cJSON_Delete(json);

    return 0;
}
