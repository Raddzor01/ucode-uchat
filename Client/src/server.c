#include "../inc/client.h"


int send_to_server(SSL *ssl, const char* request_str) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", request_str);
    cJSON_AddNumberToObject(json, "type", 0);
    char* json_str = cJSON_PrintUnformatted(json);
	SSL_write(ssl, json_str, mx_strlen(json_str));
    cJSON_Delete(json);
    free(json_str);
    return 0;
}

char* send_from_server(SSL *ssl) {
	char *buffer = malloc(1024 * sizeof(char));
	int bytes = SSL_read(ssl, &buffer, sizeof(buffer));
	if(bytes > 0) {
		buffer[bytes] = 0;
		return mx_strdup(buffer);
	}
	free(buffer);
	return NULL;
}

void check_message_from_server(SSL *ssl) {
	char *str = send_from_server(ssl);
	cJSON *json = cJSON_Parse(str);
	t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;
	t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
	if(error != 0) {
		exit(EXIT_FAILURE);
	}
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
	// server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    ip_address++;
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
}
