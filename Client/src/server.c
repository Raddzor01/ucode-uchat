#include "../inc/client.h"

void check_account_from_server(SSL *ssl);

int send_sign_up_to_server(SSL *ssl, const char* username, const char* password) {
    cJSON *json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "type", REQ_USER_SIGNUP);
    cJSON_AddStringToObject(json, "username", username);
	cJSON_AddStringToObject(json, "password", password);
    char* json_str = cJSON_PrintUnformatted(json);
	// printf("%d\n", SSL_write(ssl, json_str, mx_strlen(json_str)));
	SSL_write(ssl, json_str, mx_strlen(json_str));
    cJSON_Delete(json);
    free(json_str);
    return 0;
}

int send_login_to_server(SSL *ssl, const char* username, const char* password) {
    cJSON *json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "type", REQ_USER_LOGIN);
    cJSON_AddStringToObject(json, "username", username);
	cJSON_AddStringToObject(json, "password", password);
    char* json_str = cJSON_PrintUnformatted(json);
	// printf("%d\n", SSL_write(ssl, json_str, mx_strlen(json_str)));
	SSL_write(ssl, json_str, mx_strlen(json_str));
	check_account_from_server(info->ssl);
    cJSON_Delete(json);
    free(json_str);
    return 0;
}

char* send_from_server(SSL *ssl) {
	char *buffer = malloc(1024 * sizeof(char));
	int bytes = SSL_read(ssl, buffer, sizeof(buffer));
	int req_len = mx_strlen(buffer);
    char* len_str = mx_itoa(req_len);
    SSL_write(ssl, len_str, mx_strlen(len_str));
	printf("2\n\t%d\n\t%s\n\t%d\n", bytes, buffer, sizeof(buffer));
	if(bytes > 0) {
		buffer[bytes] = '\0';
		return buffer;
	}
	free(len_str);
	free(buffer);
	return NULL;
}

// void check_message_from_server(SSL *ssl) {
// 	char *str = send_from_server(ssl);
// 	cJSON *json = cJSON_Parse(str);
// 	if (json == NULL)
// 		pop_up_window("Error json!");
// 	t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;
// 	t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
// 	if(error != 0)
// 		pop_up_window("Error mesage!");
// }

void check_account_from_server(SSL *ssl) {
	printf("1\n");
	char *str = send_from_server(ssl);
	printf("3\n");
	cJSON *json = cJSON_Parse(str);
	if (json == NULL)
		pop_up_window("Error json!");
	printf("31\n");
	// t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;
	t_req_type type = cJSON_GetObjectItem(json, "type");
	printf("311\n");
	t_error_type error = cJSON_GetObjectItem(json, "error_code");
	printf("312\n");
	int id = cJSON_GetObjectItem(json, "id");
	printf("313\n");
	// char *username = cJSON_GetObjectItem(json, "username")->valuestring;
	char *username = cJSON_GetObjectItem(json, "username");
	printf("314\n");
	char *password = cJSON_GetObjectItem(json, "password");
	printf("315\n");
	if(error != 0)
		pop_up_window("Error mesage!");
	printf("32\n\t%d\n\t%s\n", error, username);
	cJSON_Delete(json);
    free(str);
	free(username);
	free(password);
	printf("4\n");
}

void init_ssl(SSL_CTX **ctx) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    
    *ctx = SSL_CTX_new(TLS_client_method());
    if (*ctx == NULL) {
		exit(EXIT_FAILURE);
    } 
}

void connect_ssl(SSL **ssl, int* server_fd, SSL_CTX **ctx) {
	*ssl = SSL_new(*ctx);
	SSL_set_mode(*ssl, SSL_MODE_ASYNC);
	SSL_set_fd(*ssl, *server_fd);

	if (SSL_connect(*ssl) == -1) {
        mx_printerr(strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void connect_to_server(const char* ip_address, int port, int* server_fd, SSL_CTX **ctx, SSL **ssl) {
    struct sockaddr_in server_addr;

	init_ssl(ctx);

    server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip_address);
	// inet_aton(ip_address, &server_addr.sin_addr);
	// inet_aton();
	server_addr.sin_port = htons(port);
	
	if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {		
		mx_printerr("1");
		exit(EXIT_FAILURE);
	}

	if (connect(*server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		mx_printerr("2");
		exit(EXIT_FAILURE);
	}

	connect_ssl(ssl, server_fd, ctx);

	info->ssl = *ssl;
}
