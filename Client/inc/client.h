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
 #include <strings.h>
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
  // GtkTextView *text_view;
  SSL *ssl;
  int server_socket;
  int msg_id_for_edit;
  time_t start_of_current_day;
} t_info;

// user data structure
typedef struct s_account {
  char *username;
  int id;
  int chat_id;
  int chat_count;
  char **chat_list;
  int *chat_id_list;
} t_account;

typedef struct s_msg {
  int msg_id;
  int user_id;
  char *text;
  // char *username;
  time_t date;
} t_msg;

typedef struct s_chat {
  int chat_id;
  char *chat_name;
} t_chat;

int send_file_to_server(char *filedir);
char *settime(struct tm *u);
extern GtkWidget *main_window;
extern t_info *info;
extern t_account *account;

// Interaction with the server
int send_sign_up_to_server(const char *username, const char *password);
int send_login_to_server(const char *username, const char *password);
long send_message_to_server(const char *str);
int send_exit_to_server();
int send_logout_to_server();
char *read_from_server();
void read_from_server_to_logs();
int get_user_chats();
t_msg **get_chat_messages_from_server(int chat_id);
int get_msg_id();
t_chat **find_chats_from_server(const char *str);
int create_chat_in_server(const char *chat_name, int chat_type);
int check_account_exists();
bool check_account_from_server();
int check_chat_id_from_server();
int edit_msg_in_server(int msg_id, const char *new_text);
int delete_msg_in_server(int msg_id);

// Conection to the server
void init_ssl(SSL_CTX **ctx);
void connect_ssl(SSL **ssl, int *server_fd, SSL_CTX **ctx);
void connect_to_server(const char *ip_address, int port, int *server_fd,
                       SSL_CTX **ctx, SSL **ssl);

// GTK part
void clear_box(GtkWidget *box);
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
void build_chat_window();
void pop_up_window(char *text);
void add_class(GtkWidget *widget, char *class_name);
void file_select(GtkWidget *widget, gpointer data);
void build_users(GtkWidget *grid);
void user_box(char *username, int id);
bool chech_fields(const gchar *username, const gchar *password);
void text_bubble(const char *text, int msg_id);
void delete_msg(GtkButton *button, gpointer data);
void delete_msg_id(GtkButton *button, gpointer msg_id);
void edit_msg(GtkButton *button, gpointer data);
void edit_accept(GtkButton *button, gpointer data);
void cancel_edit(GtkButton *button, gpointer data);
void change_msg_id_for_edit(GtkButton *button, gpointer msg_id);
GtkWidget* create_image_button(char* image_path, int width, int height);
void receive_bubble(const char *text, const char *name);
void create_chat_menu();
void display_users();

void create_chat(GtkButton *button, gpointer chatname);

// CSS part
void load_css();
void hog();
