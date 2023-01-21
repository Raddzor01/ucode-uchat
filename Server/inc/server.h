#pragma once

#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/cjson/inc/cJSON.h"
#include "../../libs/openssl/openssl/ssl.h"
#include "../../libs/openssl/openssl/err.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
#include <sqlite3.h>
#include <time.h>

#define PORT 8230
#define ADDR "localhost"
#define LISTEN_BACKLOG 10
#define MAX_SEND_DATA 4000
#define ERROR_LOG 1
#define INFO_LOG 0
#define SSL_CRT "Server/ssl/server_certificate.crt"
#define SSL_KEY "Server/ssl/server_key.key"

typedef struct s_user_info {
    char *username;
    char *password;
    int user_id;
}   t_user_info;
typedef struct s_client_info {
    int socket_info;
    SSL *ssl;
    t_user_info *user;
}   t_client_info;

void ssl_init(SSL_CTX **ctx);
void *threads_control(void *arg);
void server_init(struct sockaddr_in *server_address, socklen_t addr_size, int *server_socket, char *port);
void connect_new_client(SSL* ssl, int client_socket);
char *read_client_data(SSL *ssl);
char* get_client_request(SSL* ssl, int length);
char* read_client_data(SSL *ssl);
void daemon();
