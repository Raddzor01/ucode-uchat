#include "../inc/client.h"

#include <sys/ioctl.h>

int send_sign_up_to_server(const char *username, const char *password)
{

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_USER_SIGNUP);
	cJSON_AddStringToObject(json, "username", username);
	cJSON_AddStringToObject(json, "password", password);

	char *json_str = cJSON_PrintUnformatted(json);
	// printf("%d\n", SSL_write(ssl, json_str, mx_strlen(json_str)));
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	cJSON_Delete(json);
	free(json_str);

	return check_account_exists();
}

int send_login_to_server(const char *username, const char *password)
{

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_USER_LOGIN);
	cJSON_AddStringToObject(json, "username", username);
	cJSON_AddStringToObject(json, "password", password);

	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	cJSON_Delete(json);
	free(json_str);

	return check_account_from_server();
}

int send_message_to_server(const char *str)
{
	// printf("%d\t%s\n", account->id, account->username);

	// cJSON *json = cJSON_CreateObject();

	// cJSON_AddNumberToObject(json, "type", REQ_SEND_MSG);
	// cJSON_AddNumberToObject(json, "user_id", account->id);
	// cJSON_AddNumberToObject(json, "chat_id", account->chat_id);
	// cJSON_AddStringToObject(json, "message", str);
	// cJSON_AddStringToObject(json, "username", account->username);
	// cJSON_AddNumberToObject(json, "time", time(NULL));

	// char *json_str = cJSON_PrintUnformatted(json);
	// SSL_write(info->ssl, json_str, mx_strlen(json_str));

	// cJSON_Delete(json);
	// free(json_str);
	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_SEND_FILE);
	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	FILE *picture = fopen("/home/raddzor/My_git/ucode-uchat/Client/Ass/HOG.png", "rb");
	fseek(picture, 0, SEEK_END);
	long size = ftell(picture);
	rewind(picture);
	if (send(info->server_socket, &size, sizeof(size), 0) < 0)
	{
		perror("Send size error 1");
		exit(EXIT_FAILURE);
	}
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

int send_exit_from_server()
{

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_CLIENT_EXIT);

	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	cJSON_Delete(json);
	free(json_str);

	return 0;
}

char *read_from_server()
{

	char buffer[1024];
	int bytes = SSL_read(info->ssl, buffer, sizeof(buffer));

	// mx_logs(buffer, INFO_LOG);

	if (bytes > 0)
	{
		buffer[bytes] = 0;
		return mx_strdup(buffer);
	}
	return NULL;
}

void read_found_chats()
{

	char *str = read_from_server();

	mx_logs(str, INFO_LOG);
}

int get_user_chats()
{

	cJSON *json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "type", REQ_GET_CHATS);

	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	cJSON_Delete(json);
	free(json_str);

	char *str = read_from_server();
	cJSON *json_2 = cJSON_Parse(str);

	account->chat_count = cJSON_GetArraySize(json_2->child);
	account->chat_list = (char **)malloc(account->chat_count * sizeof(char *));
	account->chat_id_list = (int *)malloc(account->chat_count * sizeof(int));
	cJSON *js = NULL;

	for (int i = 0; i < account->chat_count; i++)
	{
		js = cJSON_GetArrayItem(json_2->child, i);
		account->chat_list[i] = mx_strdup(
			cJSON_GetObjectItemCaseSensitive(js, "chat_name")->valuestring);
		account->chat_id_list[i] = cJSON_GetObjectItem(js, "chat_id")->valueint;
	}

	cJSON_Delete(json_2);
	free(str);

	return 0;
}

int get_msg_id()
{

	char *str = read_from_server();
	cJSON *json = cJSON_Parse(str);
	int msg_id;

	if (json == NULL)
	{
		mx_printerr("cJSON is NULL\n");
		cJSON_Delete(json);
		free(str);
		return -1;
	}

	msg_id = cJSON_GetObjectItem(json, "message_id")->valueint;

	cJSON_Delete(json);
	free(str);

	return msg_id;
}

int find_chats_from_server(const char *str)
{

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_SEARCH_CHATS);
	cJSON_AddStringToObject(json, "search_pattern", str);

	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	cJSON_Delete(json);
	free(json_str);

	return 0;
}

int create_chat_in_server(const char *chat_name, int chat_type)
{

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_CREATE_CHAT);
	cJSON_AddStringToObject(json, "name", chat_name);
	// cJSON_AddNumberToObject(json, "user_id", account->id);
	cJSON_AddNumberToObject(json, "chat_type", chat_type);
	cJSON_AddNumberToObject(json, "time", time(NULL));

	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	cJSON_Delete(json);
	free(json_str);

	return 0;
}

int check_account_exists()
{

	char *str = read_from_server();
	cJSON *json = cJSON_Parse(str);

	if (json == NULL)
	{
		mx_printerr("cJSON is NULL\n");
		cJSON_Delete(json);
		free(str);
		return -1;
	}

	t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
	// t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

	// if (type == REQ_USER_SIGNUP) {
	// 	mx_printerr("Error in type\n");
	// 	return NULL;
	// }

	// printf("%d\n", error);

	return error;
}

bool check_account_from_server()
{

	char *str = read_from_server();
	cJSON *json = cJSON_Parse(str);

	if (json == NULL)
	{
		mx_printerr("cJSON is NULL\n");
		cJSON_Delete(json);
		free(str);
		return 0;
	}

	t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
	// t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

	// add error checks
	if (error != 0)
	{
		mx_printerr("Error in account cJSON\n");
		cJSON_Delete(json);
		free(str);
		return 0;
	}

	account->id = cJSON_GetObjectItem(json, "id")->valueint;
	account->username = mx_strdup(
		cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring);
	// char *password = cJSON_GetObjectItem(json, "password")->valuestring;
	// printf("%d\t%s\n", account->id, account->username);
	cJSON_Delete(json);
	free(str);

	// ???
	return 1;
}

int check_chat_id_from_server()
{

	char *str = read_from_server();
	cJSON *json = cJSON_Parse(str);

	if (json == NULL)
	{
		mx_printerr("cJSON is NULL\n");
		cJSON_Delete(json);
		free(str);
		return -1;
	}

	int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

	cJSON_Delete(json);
	free(str);

	return chat_id;
}

int edit_msg_in_server(int msg_id, const char *new_text)
{

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_EDIT_MESSAGE);
	cJSON_AddNumberToObject(json, "id", msg_id);
	cJSON_AddNumberToObject(json, "chat_id", account->chat_id);
	cJSON_AddStringToObject(json, "text", new_text);

	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	cJSON_Delete(json);
	free(json_str);

	return 0;
}

int delete_msg_in_server(int msg_id)
{

	cJSON *json = cJSON_CreateObject();

	cJSON_AddNumberToObject(json, "type", REQ_DEL_MESSAGE);
	cJSON_AddNumberToObject(json, "chat_id", account->chat_id);
	cJSON_AddNumberToObject(json, "message_id", msg_id);

	char *json_str = cJSON_PrintUnformatted(json);
	SSL_write(info->ssl, json_str, mx_strlen(json_str));

	cJSON_Delete(json);
	free(json_str);

	return 0;
}

t_msg **get_chat_messages_from_server(int chat_id) {

  cJSON *json = cJSON_CreateObject();

  cJSON_AddNumberToObject(json, "type", REQ_GET_CHAT_MESSAGES);
  cJSON_AddNumberToObject(json, "chat_id", chat_id);

  char *json_str = cJSON_PrintUnformatted(json);
  SSL_write(info->ssl, json_str, mx_strlen(json_str));

  cJSON_Delete(json);
  free(json_str);

  // read_from_server_to_logs();

  char *str = read_from_server();
  mx_logs(str, INFO_LOG);
  cJSON *json_2 = cJSON_Parse(str);

  // printf("%s\n", cJSON_PrintUnformatted(cJSON_GetObjectItemCaseSensitive(json_2, "messages")));

  cJSON *json_2_arr = cJSON_GetObjectItemCaseSensitive(json_2, "messages");
  int arr_size = cJSON_GetArraySize(json_2_arr);
  t_msg **msg = (t_msg **)malloc((arr_size) * sizeof(t_msg *));
  cJSON *json_temp;

  for (int i = 0; i < arr_size; i++) {
    msg[i] = (t_msg *)malloc(sizeof(t_msg));
    json_temp = cJSON_GetArrayItem(json_2_arr, i);
    // printf("%s\n", cJSON_PrintUnformatted(json_temp));
    msg[i]->text = mx_strdup(
        cJSON_GetObjectItemCaseSensitive(json_temp, "message")->valuestring);
    msg[i]->msg_id = cJSON_GetObjectItem(json_temp, "message_id")->valueint;
    // printf("%d\n", msg[i]->msg_id);
  }

  msg[arr_size] = NULL;

  cJSON_Delete(json_2);
  free(str);

  return msg;
}
