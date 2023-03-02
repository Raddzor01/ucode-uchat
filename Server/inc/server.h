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
#define MAX_BUF_SIZE 1024

#define SSL_CRT "Server/ssl/server_certificate.crt"
#define SSL_KEY "Server/ssl/server_key.key"

#define DB_NAME "Server/db/database.db"
#define SQL_NAME "Server/db/db_tables.sql"

#define TABLE_SIZE 1000

typedef struct s_user_info
{
    int id;
    char *username;
    char *password;
} t_user_info;
typedef struct s_client_info
{
    int socket_info;
    int hash_id;
    SSL *ssl;
    t_user_info *user;
    struct s_client_info *next;
} t_client_info;

extern t_client_info *hash_table[TABLE_SIZE];

typedef enum e_req_type
{
    REQ_USER_SIGNUP,
    REQ_USER_LOGIN,
    REQ_SEND_MSG,
    REQ_UNKNOWN,
    REQ_LOGOUT,
    REQ_EXIT,
} t_req_type;

typedef enum e_error_type
{
    ERR_SUCCESS,
    ERR_JSON,
    ERR_INVALID_PASSWORD,
    ERR_USER_EXISTS,
    ERR_USER_NONEXIST
} t_error_type;

SSL_CTX *ssl_ctx_init();
void *thread_control(void *arg);
int server_init(char *port);
void connect_new_client(SSL *ssl, int client_socket);
char *read_client_data(SSL *ssl);
char *get_client_request(SSL *ssl, int length);
char *read_client_data(SSL *ssl);
void daemon();
t_req_type handle_request(t_client_info *client_info, char *request);
void usage_error_check(int argc);
void log_client_conection(struct in_addr sa);

void user_signup(cJSON *json, t_client_info *client_info);
void user_login(cJSON *json, t_client_info *client_info);
void send_message(cJSON *json, t_client_info *client_info);

sqlite3 *db_open();
int db_init();
int db_execute_request(char *request);
int db_check_user_exists(char *username);
int db_get_id_by_username(char *username);

void handle_responde(cJSON *json, t_client_info *client_info);
void send_responde(SSL *ssl, t_req_type req_type, t_error_type err_code);

// attempt in map data stricture
typedef void (*request_handler)(cJSON *, t_client_info *);
typedef struct
{
    t_req_type type;
    request_handler handler;
} t_map_entry;
#define MAP_SIZE 3
static t_map_entry request_map[MAP_SIZE] = {
    {REQ_USER_SIGNUP, user_signup},
    {REQ_USER_LOGIN, user_login},
    {REQ_SEND_MSG, send_message}
};

void add_client(t_client_info *client);
t_client_info *find_client(int id);
void remove_client(int id);
