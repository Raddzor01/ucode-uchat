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

// #include "glib-2.0/include"
// #include "glib-2.0/glib/gtypes.h"
// #include "glib-2.0/glib/galloca.h"
// #include "glib-2.0/glib.h"
// #include "gtk-3.0/gdk/gdkconfig.h"
// #include "gtk-3.0/gdk/gdk.h"
// #include "gtk-3.0/gtk/gtk.h"

#include <gtk/gtk.h>

#define IP_ADDRESS "127.0.0.1"

// struct for sending to server
typedef struct s_info {
    GtkWidget *entry;
    SSL *ssl;
}   t_info;

typedef enum e_req_type {
    REQ_USER_SIGNUP,
    REQ_USER_LOGIN,
    REQ_UNKNOWN,
    REQ_LOGOUT,
    REQ_EXIT,
}   t_req_type;

typedef enum e_error_type {
    ERR_SUCCESS,
    ERR_JSON,
    ERR_USER_EXISTS
}   t_error_type;

void send_message(t_info *info, GtkTextView *text_view);
void send_button_clicked(GtkButton *button, t_info *info);
int send_to_server(SSL *ssl, const char* request_str);
void init_ssl(SSL_CTX **ctx);
void connect_ssl(SSL **ssl, int* server_fd, SSL_CTX **ctx);
void connect_to_server(const char* ip_address, int port, int* server_fd, SSL_CTX **ctx, SSL **ssl);

void check_message_from_server(SSL *ssl);
char* send_from_server(SSL *ssl);