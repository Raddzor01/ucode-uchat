#pragma once

#include "../../libs/cjson/inc/cJSON.h"
#include "../../libs/libmx/inc/libmx.h"
// #include "../../libs/libmx/inc/uchat.h"
#include "../../libs/openssl/openssl/err.h"
#include "../../libs/openssl/openssl/ssl.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// #include <sqlite3.h>
#include <time.h>

#include <gtk/gtk.h>

// local host
#define IP_ADDRESS "127.0.0.1"

#define MIN_NUMBER_OF_CHARACTERS 1
#define MAX_NUMBER_OF_CHARACTERS 16

// struct for sending to server
typedef struct s_info {
  GtkWidget *entry;
  GtkTextView *text_view;
  SSL *ssl;
} t_info;

// user data structure
typedef struct s_account {
  int id;
  char *username;
  int chat_id;
  char **chat_list;
  int chat_count;
  int *chat_id_list;
} t_account;

int send_file_to_server(char *filedir);
extern GtkWidget *main_window;
extern t_info *info;
extern t_account *account;

// Interaction with the server
int send_sign_up_to_server(const char *username, const char *password);
int send_login_to_server(const char *username, const char *password);
int send_message_to_server(const char *str);
int send_exit_from_server();
char *read_from_server();
void read_found_chats();
int get_user_chats();
int find_chats_from_server(const char *str);
int create_chat_in_server(const char *chat_name, int chat_type);
int check_account_exists();
bool check_account_from_server();

// Conection to the server
void init_ssl(SSL_CTX **ctx);
void connect_ssl(SSL **ssl, int *server_fd, SSL_CTX **ctx);
void connect_to_server(const char *ip_address, int port, int *server_fd,
                       SSL_CTX **ctx, SSL **ssl);

// GTK part
void send_message(GtkButton *button);
void find_chats(GtkWidget *entry);
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
void user_box(int number);
bool chech_fields(const gchar *username, const gchar *password);
void text_bubble(const char *text);
void delete_msg(GtkButton *button, gpointer data);
void edit_msg(GtkButton *button, gpointer data);
void edit_accept(GtkButton *button, gpointer data);
void cancel_edit(GtkButton *button, gpointer data);

// CSS part
void load_css();
void hog();
