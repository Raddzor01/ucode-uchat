#include "../../inc/server.h"

bool write_file_to_data(char *file_path, long size, int socket_info)
{
    FILE *image;
    char buffer[BUFSIZ];
    size_t bytes_received;
    long bytes_left = size;

    image = fopen(file_path, "wb");
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
        return true;
    }
    return false;
}

void update_user_photo(cJSON *json, t_client_info *client_info)
{
    char *query = NULL;
    int file_id = 0;

    char *extension = cJSON_GetObjectItemCaseSensitive(json, "extension")->valuestring;
    long time = cJSON_GetObjectItemCaseSensitive(json, "time")->valueint;
    long size = cJSON_GetObjectItemCaseSensitive(json, "size")->valueint;

    char *file_indf = mx_strjoin(mx_ltoa(time), mx_itoa(client_info->user->id));
    char *file_name = mx_strjoin(file_indf, extension);
    char *file_path = mx_strjoin(DATA_DIR, file_name);

    if(write_file_to_data(file_path, size, client_info->socket_info))
        return;
    
    query = sqlite3_mprintf("INSERT INTO files (filename, file_type, file_size, file_path, time) VALUES('%s', '%s', '%d', '%s', '%d'); ",
                            file_name, extension, size, file_path, time);
    db_execute_query(query);
    sqlite3_free(query);
    
    file_id = db_get_last_file_id(file_name, extension);

    query = sqlite3_mprintf("UPDATE users SET image_id = %d WHERE id = %d; ",
                            file_id, client_info->user->id);
    db_execute_query(query);

    mx_strdel(&file_indf);
    mx_strdel(&file_name);
    mx_strdel(&file_path);
    sqlite3_free(query);
}
