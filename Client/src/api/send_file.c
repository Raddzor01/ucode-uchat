#include "../../inc/client.h"

char *get_file_extension(char *url)
{
	char *file_name = strrchr(url, '/');
	if (file_name == NULL)
		file_name = url;
	else
		file_name++;
	return strrchr(file_name, '.');
}

int send_file_to_server(char *str)
{
	cJSON *json = cJSON_CreateObject();
	char *extension = get_file_extension(str);
	FILE *picture = fopen(str, "rb");
	
	fseek(picture, 0, SEEK_END);
	long size = ftell(picture);
	rewind(picture);

	cJSON_AddNumberToObject(json, "type", REQ_SEND_FILE_TO_SERVER);
	cJSON_AddStringToObject(json, "extension", extension);
	cJSON_AddNumberToObject(json, "size", size);
	cJSON_AddNumberToObject(json, "time", time(NULL));

	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	mx_strdel(&json_str);
	cJSON_Delete(json);

	char buffer[BUFSIZ];
	size_t bytes_read;

	while ((bytes_read = fread(buffer, sizeof(char), BUFSIZ, picture)) > 0)
	{
		if (send(info->server_socket, buffer, bytes_read, 0) < 0)
		{
			perror("Send size error 2");
			exit(EXIT_FAILURE);
		}
	}
    
	fclose(picture);
	int bytes_sent;
	if (ioctl(info->server_socket, TIOCOUTQ, &bytes_sent) == -1)
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

