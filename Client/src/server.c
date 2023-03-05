#include "../inc/client.h"

char* send_from_server();

int send_sign_up_to_server(const char* username, const char* password) {
    cJSON *json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "type", REQ_USER_SIGNUP);
    cJSON_AddStringToObject(json, "username", username);
	cJSON_AddStringToObject(json, "password", password);
    char* json_str = cJSON_PrintUnformatted(json);
	// printf("%d\n", SSL_write(ssl, json_str, mx_strlen(json_str)));
	SSL_write(info->ssl, json_str, mx_strlen(json_str));
	// mx_logs(send_from_server(), INFO_LOG);
    cJSON_Delete(json);
    free(json_str);
    return check_account_exists();
}

int send_login_to_server(const char* username, const char* password) {
    cJSON *json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "type", REQ_USER_LOGIN);
    cJSON_AddStringToObject(json, "username", username);
	cJSON_AddStringToObject(json, "password", password);
    char* json_str = cJSON_PrintUnformatted(json);
	// printf("%d\n", SSL_write(ssl, json_str, mx_strlen(json_str)));
	SSL_write(info->ssl, json_str, mx_strlen(json_str));
	bool success = check_account_from_server();
    cJSON_Delete(json);
    free(json_str);
    return success;
}

int send_message_to_server(const char* str) {
	// printf("%d\t%s\n", account->id, account->username);
    cJSON *json = cJSON_CreateObject();
	cJSON_AddNumberToObject(json, "type", REQ_SEND_MSG);
	cJSON_AddNumberToObject(json, "user_id", account->id);
	cJSON_AddNumberToObject(json, "chat_id", 0);			// zaglushka
	cJSON_AddStringToObject(json, "message", str);
	cJSON_AddStringToObject(json, "username", account->username);
    cJSON_AddNumberToObject(json, "time", 0);				// zaglushka
    char* json_str = cJSON_PrintUnformatted(json);
	// printf("%d\n", SSL_write(ssl, json_str, mx_strlen(json_str)));
	SSL_write(info->ssl, json_str, mx_strlen(json_str));
    cJSON_Delete(json);
    free(json_str);
    return 0;
}

char* send_from_server() {
	char buffer[1024];
	int bytes = SSL_read(info->ssl, buffer, sizeof(buffer));
	mx_logs(buffer, INFO_LOG);
	if(bytes > 0) {
		buffer[bytes] = 0;
		return mx_strdup(buffer);
	}
	return NULL;
}

int check_account_exists() {
	
	char *str = send_from_server();
	cJSON *json = cJSON_Parse(str);

	if (json == NULL) {
		mx_printerr("cJSON is NULL\n");
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

bool check_account_from_server() {
	
	char *str = send_from_server();
	cJSON *json = cJSON_Parse(str);

	if (json == NULL) {
		mx_printerr("cJSON is NULL\n");
		return 0;
	}

	t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
	// t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

	// add error checks
	if (error != 0) {
		mx_printerr("Error in account cJSON\n");
		return 0;
	}

	account->id = cJSON_GetObjectItem(json, "id")->valueint;
	account->username = cJSON_GetObjectItem(json, "username")->valuestring;
	// char *password = cJSON_GetObjectItem(json, "password")->valuestring;
	// printf("%d\t%s\n", account->id, account->username);
	cJSON_Delete(json);
	free(str);

	// ???
	return 1;
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
