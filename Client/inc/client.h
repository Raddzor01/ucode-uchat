#pragma once

#include "../../libs/cjson/inc/cJSON.h"
#include "../../libs/libmx/inc/libmx.h"
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
typedef struct s_info
{
    GtkWidget *entry;
    // GtkTextView *text_view;
    SSL *ssl;
    int server_socket;
    int msg_id_for_edit;
    time_t current_day_time;
} t_info;

typedef struct s_msg
{
    int msg_id;
    int user_id;
    char *text;
    char *username;
    int time;
    struct s_msg *next;
} t_msg;

typedef struct s_chat
{
    int id;
    char *name;
    t_msg *messages;
    struct s_chat *next;
} t_chat;

// user data structure
typedef struct s_account
{
    char *username;
    int id;

    pthread_t server_update_thread;
    pthread_mutex_t mutex;
    bool is_busy;

    t_chat *chats;
    t_chat *current_chat;
} t_account;

int send_file_to_server(char *filedir);
extern GtkWidget *main_window;
extern t_info *info;
extern t_account *account;

void msg_push_back(t_msg **list, t_msg *new_node);
void msg_clear_list(t_msg **list);
void msg_clear_single_node(t_msg **node);
int msg_list_size(t_msg *list);
t_msg *msg_get_by_id(t_msg *list, int id);
void msg_pop_by_message_id(t_msg **list, int msg_id);
void msg_pop_index(t_msg **list, int index);
void msg_pop_back(t_msg **head);
void msg_pop_front(t_msg **head);
t_msg *msg_get_last_message(t_msg *list);

void chat_push_back(t_chat **list, t_chat *new_node);
void chat_push_front(t_chat **list, t_chat *new_node);
t_chat *chat_get_chat_by_id(t_chat *list, int chat_id);
void chat_clear_single_node(t_chat **list);
void chat_clear_list(t_chat **list);
int chat_list_size(t_chat *list);

void *server_update_thread();

// Interaction with the server
int send_sign_up_to_server(const char *username, const char *password);
int send_login_to_server(const char *username, const char *password);
time_t send_message_to_server(const char *str);
int send_exit_to_server();
int send_logout_to_server();
char *read_from_server();
void read_from_server_to_logs();
int get_user_chats();
int join_to_found_chat(int chat_id);
t_msg *get_chat_messages_from_server(int chat_id);
int get_msg_id();
t_chat *find_chats_from_server(const char *str);
int create_chat_in_server(const char *chat_name, int chat_type);
int check_account_exists();
bool check_account_from_server();
int check_chat_id_from_server();
int edit_msg_in_server(int msg_id, const char *new_text);
int delete_msg_in_server(int msg_id);
int get_last_msg_id_from_server(int chat_id);
t_msg *get_msg_by_id_from_server(int msg_id, int chat_id);
gboolean update_chatlist_from_thread(gpointer user_data);

// Conection to the server
void init_ssl(SSL_CTX **ctx);
void connect_ssl(SSL **ssl, int server_fd, SSL_CTX **ctx);
void connect_to_server(const char *ip_address, int port, int server_fd,
                       SSL_CTX **ctx, SSL **ssl);

// GTK part
void clear_box(GtkWidget *box);
void send_message(GtkButton *button);
void find_chats(GtkWidget *entry);
void chat_window();
void create_new_window(char *title, int width, int height, bool resizable);
void log_menu(GtkWidget *__attribute__((unused)) widget);
void clear_window(GtkWidget *window);
void build_login();
void sign_up_menu(GtkWidget *__attribute__((unused)) widget);
void build_signup();
GtkWidget *get_widget_by_name_r(GtkWidget *container, char *name);
void build_chat_window();
void pop_up_window(char *text);
void add_class(GtkWidget *widget, char *class_name);
void file_select(GtkWidget *widget, gpointer data);
void build_users(GtkWidget *grid);
void user_box(t_chat *chat, bool is_search);
bool chech_fields(const gchar *username, const gchar *password);
void text_bubble(const char *text, int msg_id, time_t msg_time);
void delete_msg(GtkButton *__attribute__((unused)) button, gpointer data);
void delete_msg_id(GtkButton *__attribute__((unused)) button, gpointer msg_id);
void edit_msg(GtkButton *__attribute__((unused)) button, gpointer data);
void edit_accept(GtkButton *__attribute__((unused)) button, gpointer data);
void cancel_edit(GtkButton *__attribute__((unused)) button, gpointer data);
void change_msg_id_for_edit(GtkButton *__attribute__((unused)) button, gpointer msg_id);
GtkWidget *create_image_button(char *image_path, int width, int height);
void receive_bubble(const char *text, const char *name);
void create_chat(GtkButton *__attribute__((unused)) button, gpointer chatname);
void join_chat(GtkWidget *__attribute__((unused)) widget, gpointer id);
void receive_bubble(const char *text, const char *name);
void create_chat_menu();
void display_users();
char *get_send_time_str(time_t send_time);

// CSS part
void load_css();
void hog();
