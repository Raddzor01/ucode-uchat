#include "../../inc/client.h"

bool write_file_to_data(char *file_path, long size)
{
    FILE *image;
    char buffer[1024];
    size_t bytes_received = 0;
    long bytes_left = size;

    image = fopen(file_path, "wb");

    if (image == NULL)
        return true;
    
    while (bytes_left > 0)
    {
        bytes_received = recv(info->server_socket, buffer, BUFSIZ, 0);
        if (bytes_received < 0)
        {
            mx_logs("Getting file error", LOG_ERROR);
            return true;
        }
        fwrite(buffer, sizeof(char), bytes_received, image);
        bytes_left -= bytes_received;
    }

    // pthread_mutex_unlock(&account->mutex);
    fclose(image);

    int bytes_pending;
    if (ioctl(info->server_socket, FIONREAD, &bytes_pending) == -1)
    {
        mx_logs("Ioctl error", LOG_ERROR);
        return true;
    }
    if (bytes_pending > 0)
    {
        remove(file_path);
        printf("File is not fully received\n");
        return true;
    }
    return false;
}

char *get_image_from_server(int image_id)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_SEND_FILE_TO_CLIENT);
    cJSON_AddNumberToObject(json, "image_id", image_id);
    char *json_str = cJSON_PrintUnformatted(json);

    // pthread_mutex_lock(&account->mutex);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    char *user_image_name = mx_strjoin(cJSON_GetObjectItemCaseSensitive(json, "filename")->valuestring, cJSON_GetObjectItemCaseSensitive(json, "extension")->valuestring);
    char *user_image_path = mx_strjoin(DATA_DIR, user_image_name);
    long size = cJSON_GetObjectItemCaseSensitive(json, "size")->valueint;    

    if (access(user_image_path, F_OK) == 0)
        return user_image_path;

    if (write_file_to_data(user_image_path, size))
        return NULL;

    mx_strdel(&user_image_name);
    
    return user_image_path;
}
