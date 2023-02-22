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

#define IP_ADDRESS "127.0.0.1"

extern GtkWidget *main_window;

// struct for sending to server
typedef struct s_info {
    GtkWidget *entry;
    GtkTextView *text_view;
    SSL *ssl;
}   t_info;

void send_message(GtkButton *button, t_info *info);
void send_button_clicked(GtkButton *button, t_info *info);
int send_to_server(SSL *ssl, const char* request_str);
void init_ssl(SSL_CTX **ctx);
void connect_ssl(SSL **ssl, int* server_fd, SSL_CTX **ctx);
void connect_to_server(const char* ip_address, int port, int* server_fd, SSL_CTX **ctx, SSL **ssl);
void chat_window(t_info *info);
GtkWidget* create_new_window(char *title, int width, int height, bool resizable);
void log_menu(t_info *info);
