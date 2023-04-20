#include "../../inc/server.h"

int send_file_to_client(char *file_path, t_client_info *client_info)
{
	FILE *picture = NULL;
	char buffer[BUFSIZ];
	size_t bytes_read = 0;
	int bytes_sent = 0;

	picture = fopen(file_path, "rb");
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
	
	if (ioctl(client_info->socket_info, TIOCOUTQ, &bytes_sent) == -1)
		perror("ioctl");

	if (bytes_sent > 0)
		fprintf(stderr, "Warning: there are still %d bytes in the send buffer\n", bytes_sent);

	return 0;
}

void send_image(cJSON *json, t_client_info *client_info)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *query = NULL;

	char *file_path = NULL;
	char *filename = NULL;
	char *extension = NULL;
	long file_size = 0;

	cJSON *responde_json = NULL;
	char *json_str = NULL;

	int image_id = cJSON_GetObjectItemCaseSensitive(json, "image_id")->valueint;

	db = db_open();
	query = sqlite3_mprintf("SELECT filename, file_type, file_size, file_path FROM files "
							"WHERE id = %d ;",
							image_id);
	stmt = db_execute_query_and_return_stmt(query, db);

	filename = mx_strdup((const char *)sqlite3_column_text(stmt, 0));
	extension = mx_strdup((const char *)sqlite3_column_text(stmt, 1));
	file_size = sqlite3_column_int64(stmt, 2);
	file_path = mx_strdup((const char *)sqlite3_column_text(stmt, 3));

	responde_json = cJSON_CreateObject();

	cJSON_AddNumberToObject(responde_json, "type", REQ_SEND_FILE_TO_CLIENT);
	cJSON_AddStringToObject(responde_json, "filename", filename);
	cJSON_AddStringToObject(responde_json, "extension", extension);
	cJSON_AddNumberToObject(responde_json, "size", file_size);
	json_str = cJSON_PrintUnformatted(responde_json);

	SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

	mx_strdel(&extension);
	mx_strdel(&filename);
	mx_strdel(&json_str);
	cJSON_Delete(responde_json);

	send_file_to_client(file_path, client_info);

	mx_strdel(&file_path);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}
