#include "../../inc/server.h"

bool write_file_to_data(char *filename, long file_size, SSL *ssl)
{
    int fd, bytes_received, bytes_written;
    int bytes_total = 0;
    char buffer[BUFFER_SIZE];

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
        return -1;
    }

    while (bytes_total < file_size) {
        bytes_received = SSL_read(ssl, buffer, BUFFER_SIZE);
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
            continue;

        if (bytes_received <= 0) {
            fprintf(stderr, "Error receiving file: %s\n", ERR_error_string(ERR_get_error(), NULL));
            close(fd);
            return -1;
        }

        bytes_written = write(fd, buffer, bytes_received);
        if (bytes_written <= 0) {
            fprintf(stderr, "Error writing file: %s\n", strerror(errno));
            close(fd);
            return -1;
        }

        bytes_total += bytes_received;
    }
    close(fd);
    return false;
}

void update_chat_photo(cJSON *json, t_client_info *client_info)
{
    char *query = NULL;
    int file_id = 0;

    char *extension = mx_strdup(cJSON_GetObjectItemCaseSensitive(json, "extension")->valuestring);
    long time = cJSON_GetObjectItemCaseSensitive(json, "time")->valueint;
    long size = cJSON_GetObjectItemCaseSensitive(json, "size")->valueint;
    int chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;

    char *file_indf = mx_strjoin(mx_ltoa(time), mx_itoa(client_info->user->id));
    char *file_name = mx_strjoin(file_indf, extension);
    char *file_path = mx_strjoin(DATA_DIR, file_name);

    if(write_file_to_data(file_path, size, client_info->ssl))
        return;
    
    query = sqlite3_mprintf("INSERT INTO files (filename, file_size, file_path) VALUES('%s', %d, '%s'); ",
                            file_name, size, file_path);
    db_execute_query(query);
    sqlite3_free(query);
    
    file_id = db_get_last_file_id(file_name);

    query = sqlite3_mprintf("UPDATE chats SET image_id = %d WHERE id = %d; ",
                            file_id, chat_id);
    db_execute_query(query);

    mx_strdel(&extension);
    mx_strdel(&file_indf);
    mx_strdel(&file_name);
    mx_strdel(&file_path);
    sqlite3_free(query);
}

