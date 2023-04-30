#include "../../inc/client.h"

t_user **get_chat_users_from_server(const int chat_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_CHAT_USERS);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    sem_wait(&account->semaphore);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
    sem_post(&account->semaphore);
    json = cJSON_Parse(json_str);

    cJSON *users = cJSON_GetObjectItemCaseSensitive(json, "users");
    int users_count = cJSON_GetArraySize(users);

    t_user **chat_users = (t_user **)malloc(sizeof(t_user *) * users_count);

    for (int i = 0; i < users_count; i++)
    {
        cJSON *temp = cJSON_GetArrayItem(users, i);
        chat_users[i] = (t_user *)malloc(sizeof(t_user));
        chat_users[i]->id = cJSON_GetObjectItem(temp, "id")->valueint;
        chat_users[i]->username = mx_strdup(cJSON_GetObjectItem(temp, "username")->valuestring);
        chat_users[i]->image_id = cJSON_GetObjectItem(temp, "image_id")->valueint;
        chat_users[i]->privilege = cJSON_GetObjectItem(temp, "privilege")->valueint;
    }
    chat_users[users_count] = NULL;

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return chat_users;
}
