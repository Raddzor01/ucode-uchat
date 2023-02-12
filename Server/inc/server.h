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
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
#include <sqlite3.h>
#include <time.h>

#define LISTEN_BACKLOG 10
#define MAX_SEND_DATA 1024

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

typedef enum e_req_type {
    REQ_COMMON_MESSAGE,
    REQ_TEST_RESPONDE,
    REQ_LOGOUT,
    REQ_EXIT,
}   t_req_type;

SSL_CTX *ssl_ctx_init();
void *thread_control(void *arg);
int server_init(char *port);
void connect_new_client(SSL* ssl, int client_socket);
char *read_client_data(SSL *ssl);
char* get_client_request(SSL* ssl, int length);
char* read_client_data(SSL *ssl);
void daemon();
t_req_type handle_request(t_client_info *client_info, char *request);
void usage_error_check(int argc);
void log_client_conection(struct in_addr sa);

void handle_responde(cJSON *json, t_client_info *client_info);
