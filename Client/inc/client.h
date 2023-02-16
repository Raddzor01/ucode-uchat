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

void send_message(GtkWidget *entry, GtkTextView *text_view);
void send_button_clicked(GtkButton *button, GtkWidget *entry);
int send_to_server(SSL *ssl, const char* request_str);
void init_ssl(SSL_CTX **ctx);
void connect_ssl(SSL **ssl, int* server_fd, SSL_CTX **ctx);
void connect_to_server(const char* ip_address, int port, int* server_fd, SSL_CTX **ctx, SSL **ssl);
