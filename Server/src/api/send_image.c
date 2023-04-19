#include "../../inc/server.h"

int send_file_to_client(long file_size, char *file_path, t_client_info *client_info)
{

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_SEND_FILE_TO_CLIENT);
	cJSON_AddNumberToObject(json, "size", file_size);
	char *json_str = cJSON_PrintUnformatted(json);

	SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

	mx_strdel(&json_str);
	cJSON_Delete(json);

	char buffer[BUFSIZ];
	size_t bytes_read;
	FILE *picture = fopen(file_path, "rb");
	if(picture == NULL)
		return 1;

	while ((bytes_read = fread(buffer, sizeof(char), BUFSIZ, picture)) > 0)
	{
		if (send(client_info->socket_info, buffer, bytes_read, 0) < 0)
		{
			perror("Send size error 2");
			exit(EXIT_FAILURE);
		}
	}

	fclose(picture);
	int bytes_sent;
	if (ioctl(client_info->socket_info, TIOCOUTQ, &bytes_sent) == -1)
	{
		perror("ioctl");
		exit(EXIT_FAILURE);
	}
	if (bytes_sent > 0)
	{
		fprintf(stderr, "Warning: there are still %d bytes in the send buffer\n", bytes_sent);
	}
	return 0;
}

void send_image(cJSON *json, t_client_info *client_info)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *query = NULL;

	char *file_path = NULL;

	int image_id = cJSON_GetObjectItemCaseSensitive(json, "image_id")->valueint;

	db = db_open();
	query = sqlite3_mprintf("SELECT file_size, file_path FROM files "
							"WHERE id = %d ;",
							image_id);
	stmt = db_execute_query_and_return_stmt(query, db);

	long file_size = sqlite3_column_int64(stmt, 0);
	file_path = mx_strdup((const char *)sqlite3_column_text(stmt, 1));

	send_file_to_client(file_size, file_path, client_info);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	mx_strdel(&file_path);
}
