#include "../../inc/client.h"

int edit_username_in_server(const char *new_username)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_EDIT_USERNAME);
    cJSON_AddStringToObject(json, "new_username", new_username);
    char *json_str = cJSON_PrintUnformatted(json);

#ifdef MACOS_VER
    sem_wait(account->semaphore);
#else
    sem_wait(&account->semaphore);
#endif
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    json_str = read_from_server();
#ifdef MACOS_VER
    sem_post(account->semaphore);
#else
    sem_post(&account->semaphore);
#endif
    json = cJSON_Parse(json_str);

    t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;

    if (error == 0)
        pop_up_window("Login changed successful");
    else
    {
        pop_up_window("Error trying to change login\nTry again");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    mx_strdel(&account->username);

    pthread_mutex_lock(&account->mutex);
    account->username = mx_strdup(new_username);
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    free(json_str);

    return 0;
}

int edit_password_in_server(const char *new_password)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_EDIT_PASSWORD);
    cJSON_AddStringToObject(json, "new_password", new_password);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;

    if (error == 0)
        pop_up_window("Password changed successful");
    else
    {
        pop_up_window("Error trying to change password\nTry again");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    cJSON_Delete(json);
    free(json_str);

    return 0;
}
