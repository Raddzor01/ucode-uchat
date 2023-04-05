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
#define MAX_BUF_SIZE 4000

#define SSL_CRT "Server/ssl/server_certificate.crt"
#define SSL_KEY "Server/ssl/server_key.key"

#define DB_NAME "Server/db/database.db"
#define SQL_NAME "Server/db/db_tables.sql"

#define DATA_DIR "Server/db/data/"

#define TABLE_SIZE 100

// max size 8 mb
#define MAX_FILE_SIZE 8388608

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
void send_file(cJSON *json, t_client_info *client_info);
void create_chat(cJSON *json, t_client_info *client_info);
void get_chats(cJSON *chat_info, t_client_info *client_info);
void search_chats(cJSON *client_json, t_client_info *client_info);
void edit_message(cJSON *client_json, t_client_info *client_info);
void delete_message(cJSON *json, t_client_info *client_info);
void join_chat(cJSON *chat_info, t_client_info *client_info);

sqlite3 *db_open();
int db_init();
int db_check_exist();
int db_execute_query(const char *sql_query);
sqlite3_stmt *db_execute_query_and_return_stmt(const char *sql_query, sqlite3 *db);
int db_check_user_exists(char *username);
int db_get_id_by_username(char *username);
bool db_check_chat_exists(int id);
bool db_check_chat_membership(int chat_id, int user_id);

void handle_responde(cJSON *json, t_client_info *client_info);
void send_responde(SSL *ssl, t_req_type req_type, t_error_type err_code);

typedef void (*request_handler)(cJSON *, t_client_info *);
typedef struct
{
    t_req_type type;
    request_handler handler;
} t_map_entry;

#define MAP_SIZE 9
static t_map_entry request_map[MAP_SIZE] = {
    {REQ_USER_SIGNUP, user_signup},
    {REQ_USER_LOGIN, user_login},
    {REQ_SEND_MSG, send_message},
    {REQ_SEND_FILE, send_file},
    {REQ_CREATE_CHAT, create_chat},
    {REQ_DEL_MESSAGE, delete_message},
    {REQ_GET_CHATS, get_chats},
    {REQ_SEARCH_CHATS, edit_message},
    {REQ_JOIN_CHAT, join_chat}
};

void add_client(t_client_info *client);
t_client_info *find_client(int id);
void remove_client(int id);
