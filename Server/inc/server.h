#pragma once

#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/cjson/inc/cJSON.h"
#include "../../libs/openssl/openssl/ssl.h"
#include "../../libs/openssl/openssl/err.h"
#include "../../libs/Sqlite3/sqlite3.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
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
#include <time.h>

#define BUFFER_SIZE 1024

#define LISTEN_BACKLOG 10
#define MAX_BUF_SIZE 4000

#ifdef MACOS_VER
#define SSL_CRT "Server/ssl/key.pem"
#else
#define SSL_CRT "Server/ssl/server_certificate.crt"
#endif

#ifdef MACOS_VER
#define SSL_KEY "Server/ssl/key.pem"
#else
#define SSL_KEY "Server/ssl/server_key.key"
#endif

#define DB_NAME "Server/db/database.db"
#define SQL_NAME "Server/db/db_tables.sql"

#define DATA_DIR "Server/db/data/"

#define TABLE_SIZE 100

typedef struct s_user_info
{
    int id;
    char *username;
    unsigned char *password;
    int image_id;
    char *filename;
    char *extension;
    char *file_path;
} t_user_info;
typedef struct s_client_info
{
    int socket_info;
    int hash_id;
    SSL *ssl;
    t_user_info *user;
    struct s_client_info *next;
} t_client_info;

#include "request_map.h"

extern t_client_info *hash_table[TABLE_SIZE];

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
bool write_file_to_data(char *filename, long file_size, SSL *ssl);

sqlite3 *db_open();
int db_init();
int db_check_exist();
int db_execute_query(const char *sql_query);
sqlite3_stmt *db_execute_query_and_return_stmt(const char *sql_query, sqlite3 *db);
int db_check_user_exists(char *username);
int db_get_id_by_username(char *username);
bool db_check_chat_exists(int id);
bool db_check_chat_membership(int chat_id, int user_id);
int db_get_last_message_id(int user_id, int chat_id);
int db_get_last_file_id(char *filename);
int db_get_last_join_chat_id(int user_id, int chat_id);
t_privilege_type db_check_user_privilege(int user_id, int chat_id);

void handle_responde(cJSON *json, t_client_info *client_info);
void send_responde(SSL *ssl, t_req_type req_type, t_error_type err_code);

void add_client(t_client_info *client);
t_client_info *find_client(int id);
void remove_client(int id);
