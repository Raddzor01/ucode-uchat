#include "../../inc/client.h"

int check_chat_id_from_server()
{
    char *json_str = read_from_server();
#ifdef MACOS_VER
            sem_post(account->semaphore);
#else
            sem_post(&account->semaphore);
#endif

    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        pop_up_window("Error while creating chat\n Try again\n");
        cJSON_Delete(json);
        mx_strdel(&json_str);
        return -1;
    }

    int error_code = cJSON_GetObjectItem(json, "error_code")->valueint;
    if (error_code != ERR_SUCCESS)
    {
        if (error_code == ERR_CHAT_EXIST)
        {
            pop_up_window("Chat already exists!");
            cJSON_Delete(json);
            mx_strdel(&json_str);
            return -1;
        }
        else
        {
            pop_up_window("Error while creating chat\n Try again\n");
            cJSON_Delete(json);
            mx_strdel(&json_str);
            return -1;
        }
    }

    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

    cJSON_Delete(json);
    free(json_str);

    return chat_id;
}

int create_chat_in_server(const char *chat_name, int chat_type)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_CREATE_CHAT);
    cJSON_AddStringToObject(json, "name", chat_name);
    cJSON_AddNumberToObject(json, "chat_type", chat_type);
    cJSON_AddNumberToObject(json, "time", time(NULL));
    char *json_str = cJSON_PrintUnformatted(json);

#ifdef MACOS_VER
    sem_wait(account->semaphore);
#else
    sem_wait(&account->semaphore);
#endif
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return check_chat_id_from_server();
}
