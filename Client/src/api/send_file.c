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

time_t send_file_to_server(char *str)
{
	time_t send_time = time(NULL);

	char *extension = get_file_extension(str);
	if(mx_strcmp(extension, ".jpg") != 0 && mx_strcmp(extension, ".jpeg") != 0 && mx_strcmp(extension, ".png") != 0)
		pop_up_window("Unsupported image format. Choose another one");
	FILE *picture = fopen(str, "rb");

	fseek(picture, 0, SEEK_END);
	long size = ftell(picture);
	fclose(picture);

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_SEND_FILE_TO_SERVER);
	cJSON_AddStringToObject(json, "extension", extension);
	cJSON_AddNumberToObject(json, "size", size);
	cJSON_AddNumberToObject(json, "time", send_time);
	char *json_str = cJSON_PrintUnformatted(json);

	sem_wait(&account->semaphore);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	mx_strdel(&json_str);
	cJSON_Delete(json);

	int fd, bytes_read, bytes_sent;
	char buffer[BUFFER_SIZE];

	fd = open(str, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "Error opening file %s: %s\n", str, strerror(errno));
		return -1;
	}

	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		bytes_sent = 0;
		while (bytes_sent < bytes_read)
		{
			int ret = SSL_write(info->ssl, buffer + bytes_sent, bytes_read - bytes_sent);
			if (ret < 0)
			{
				fprintf(stderr, "Error sending file: %s\n", ERR_error_string(ERR_get_error(), NULL));
				close(fd);
				return -1;
			}
			bytes_sent += ret;
		}
	}
	close(fd);

	sem_post(&account->semaphore);
	return send_time;
}
