#include "../../inc/client.h"


bool write_file_to_data(char *file_path, long size, int socket_info)
{
    FILE *image;
    char buffer[BUFSIZ];
    size_t bytes_received = 0;
    long bytes_left = size;

    image = fopen(file_path, "wb");

    sleep(1);
    while (bytes_left > 0)
    {
        bytes_received = recv(socket_info, buffer, BUFSIZ, 0);
        if (bytes_received < 0)
        {
            mx_logs("Getting file error", ERROR_LOG);
            return true;
        }
        fwrite(buffer, sizeof(char), bytes_received, image);
        bytes_left -= bytes_received;
    }

    pthread_mutex_unlock(&account->mutex);
    fclose(image);

    int bytes_pending;
    if (ioctl(socket_info, FIONREAD, &bytes_pending) == -1)
    {
        mx_logs("Ioctl error", ERROR_LOG);
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

bool get_image_from_server(int image_id)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_SEND_FILE_TO_CLIENT);
    cJSON_AddNumberToObject(json, "image_id", image_id);
    char *json_str = cJSON_PrintUnformatted(json);

    pthread_mutex_lock(&account->mutex);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    long size = cJSON_GetObjectItemCaseSensitive(json, "size")->valueint;

    if(write_file_to_data(account->image_path, size, info->server_socket))
        return true;
    return false;
}
