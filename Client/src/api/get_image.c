#include "../../inc/client.h"

bool write_file_to_data(char *filename, long file_size)
{
    int fd, bytes_received, bytes_written;
    int bytes_total = 0;
    char buffer[BUFFER_SIZE];

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
        return true;
    }

    while (bytes_total < file_size)
    {
        bytes_received = SSL_read(info->ssl, buffer, BUFFER_SIZE);
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
            continue;

        if (bytes_received <= 0)
        {
            fprintf(stderr, "Error receiving file: %s\n", ERR_error_string(ERR_get_error(), NULL));
            close(fd);
            return true;
        }

        bytes_written = write(fd, buffer, bytes_received);
        if (bytes_written <= 0)
        {
            fprintf(stderr, "Error writing file: %s\n", strerror(errno));
            close(fd);
            return true;
        }

        bytes_total += bytes_received;
    }

    close(fd);
    return false;
}

char *get_image_from_server(int image_id)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_CHECK_FILE);
    cJSON_AddNumberToObject(json, "image_id", image_id);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    char *user_image_name = mx_strdup(cJSON_GetObjectItemCaseSensitive(json, "filename")->valuestring);
    char *user_image_path = mx_strjoin(DATA_DIR, user_image_name);
    long size = cJSON_GetObjectItemCaseSensitive(json, "size")->valueint;    

    if (access(user_image_path, F_OK) == 0)
        return user_image_path;

    json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_SEND_FILE_TO_CLIENT);
    cJSON_AddNumberToObject(json, "image_id", image_id);
    json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    if (write_file_to_data(user_image_path, size) == 1)
        return DEFAULT_IMAGE;

    mx_strdel(&user_image_name);
    
    return user_image_path;
}
