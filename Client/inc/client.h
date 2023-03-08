#pragma once

#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/cjson/inc/cJSON.h"
#include "../../libs/openssl/openssl/ssl.h"
#include "../../libs/openssl/openssl/err.h"

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
// #include <sqlite3.h>
#include <time.h>

#include <gtk/gtk.h>

// local host
#define IP_ADDRESS "127.0.0.1"

// struct for sending to server
typedef struct s_info {
    GtkWidget *entry;
    GtkTextView *text_view;
    SSL* ssl;
}   t_info;

// user data structure
typedef struct s_account {
    int id;
    char* username;
}   t_account;

typedef enum e_req_type
{
    REQ_USER_SIGNUP,
    REQ_USER_LOGIN,
    REQ_SEND_MSG,
    REQ_SEND_FILE,
    REQ_CREATE_CHAT,
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
    ERR_USER_NONEXIST,
    ERR_CHAT_EXIST
} t_error_type;

int send_file_to_server(char *filedir);
extern GtkWidget *main_window;
extern t_info *info;
extern t_account *account;

// Interaction with the server
int send_sign_up_to_server(const char* username, const char* password);
int send_login_to_server(const char* username, const char* password);
int send_message_to_server(const char* str);
char* send_from_server();
int check_account_exists();
bool check_account_from_server();


// Conection to the server
void init_ssl(SSL_CTX **ctx);
void connect_ssl(SSL **ssl, int* server_fd, SSL_CTX **ctx);
void connect_to_server(const char* ip_address, int port, int* server_fd, SSL_CTX **ctx, SSL **ssl);

// GTK part
void send_message(GtkButton *button);
void send_button_clicked(GtkButton *button);
void chat_window();
void create_new_window(char *title, int width, int height, bool resizable);
void log_menu(GtkWidget *widget);
void clear_window(GtkWidget *window);
void build_login();
void sign_up_menu(GtkWidget *widget);
void build_signup();
GtkWidget *get_widget_by_name_r(GtkWidget *container, char *name);
void build_chat_window(GtkWidget *grid);
void pop_up_window(char *text);
void add_class(GtkWidget *widget, char *class_name);
void file_select(GtkWidget *widget, gpointer data);
void build_users(GtkWidget *grid);
void user_box(GtkWidget *widget);
bool chech_fields(const gchar *username, const gchar *password);

// CSS part
void load_css();
void hog();

