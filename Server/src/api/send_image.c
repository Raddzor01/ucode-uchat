#include "../../inc/server.h"

int send_file_to_client(char *str, t_client_info *client_info)
{
    int fd, bytes_read, bytes_sent;
    char buffer[BUFFER_SIZE];

    // Open the file to be sent
    fd = open(str, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error opening file %s: %s\n", str, strerror(errno));
        return true;
    }

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_sent = 0;
        while (bytes_sent < bytes_read) {
            int ret = SSL_write(client_info->ssl, buffer + bytes_sent, bytes_read - bytes_sent);
            if (ret < 0) {
                fprintf(stderr, "Error sending file: %s\n", ERR_error_string(ERR_get_error(), NULL));
                close(fd);
                return true;
            }
            bytes_sent += ret;
        }
    }
	close(fd);
	return false;
}

void send_image(cJSON *json, t_client_info *client_info)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *query = NULL;

	char *file_path = NULL;

	int image_id = cJSON_GetObjectItemCaseSensitive(json, "image_id")->valueint;

	db = db_open();
	query = sqlite3_mprintf("SELECT file_path FROM files "
							"WHERE id = %d ;",
							image_id);
	stmt = db_execute_query_and_return_stmt(query, db);

	file_path = mx_strdup((const char *)sqlite3_column_text(stmt, 0));
	
	send_file_to_client(file_path, client_info);

	mx_strdel(&file_path);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}
