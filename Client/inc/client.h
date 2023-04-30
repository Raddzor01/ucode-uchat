#pragma once

#include "../../libs/cjson/inc/cJSON.h"
#include "../../libs/libmx/inc/libmx.h"
#include "../../libs/openssl/openssl/err.h"
#include "../../libs/openssl/openssl/ssl.h"
#include "../../libs/openssl/openssl/sha.h"

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
#include <sys/ioctl.h>
#include <semaphore.h>

#include <gtk/gtk.h>

#define BUFFER_SIZE 1024

#define DATA_DIR "Client/data/"

#define DEFAULT_IMAGE "Client/icons/default_user.png"
#define SAVED_IMAGE "Client/icons/saved.png"
#define DEFAULT_CHAT_IMAGE "Client/icons/default_chat.png"

#define MIN_NUMBER_OF_CHARACTERS 1
#define MAX_NUMBER_OF_CHARACTERS 64

#define MAX_NUMBER_OF_CHAR_FOR_CHAT_NAME 14
#define MAX_NUMBER_OF_CHAR_FOR_LAST_MSG 24

#define CHAT_DIDNT_EXISTS -2
#define CHAT_ERROR -1

#define SAVED_NAME "Saved"
#define MSG_NAME "msg_name_id_"
#define USER_NAME "chat_user_id"

// struct for sending to server
typedef struct s_info
{
    int server_socket;
    SSL_CTX *ctx;
    SSL *ssl;

    int port;
    char *ip_address;

    GtkWidget *entry;
    // GtkTextView *text_view;

    int msg_id_for_edit;
    time_t current_day_time;
} t_info;

// user data structure
typedef struct s_account
{
    char *username;
    unsigned char *password;
    int id;
    int image_id;
    char *image_path;

    pthread_t server_update_thread;
    pthread_t server_online_thread;
    pthread_mutex_t mutex;

    #ifdef MACOS_VER
        sem_t *semaphore;
    #else
        sem_t semaphore;
    #endif

    t_chat *chats;
    t_chat *current_chat;
} t_account;

time_t send_file_to_server(char *filedir);
extern GtkWidget *main_window;
extern t_info *info;
extern t_account *account;
extern bool username_display;
extern bool window_check;

void *server_update_thread();
char *get_user_image(int image_id, int pfp_type);
bool check_server_online();
void *server_online_check_thread();
void change_chat_image(GtkWidget *button);
char *get_file_extension(char *url);
void change_chat_image(GtkWidget *button);
time_t update_chat_photo(char *str, int chat_id);
// Interaction with the server
int send_sign_up_to_server(const char *username, const unsigned char *password);
int send_login_to_server(const char *username, const unsigned char *password);
time_t send_message_to_server(const char *str);
int send_exit_to_server();
int send_logout_to_server();
char *read_from_server();
void read_from_server_to_logs();
t_chat *get_user_chats();
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
char *get_image_from_server(int image_id);
int edit_username_in_server(const char *new_username);
int edit_password_in_server(const char *new_password);
int delete_account_in_server();
int delete_chat_in_server();
t_user **get_chat_users_from_server(const int chat_id);
int edit_chat_name_in_server(const char *new_name);
int kick_user_in_server(int chat_id, int user_id);

// Conection to the server
bool init_ssl(SSL_CTX **ctx);
bool connect_ssl(SSL **ssl, int server_fd, SSL_CTX **ctx);
bool connect_to_server(const char *ip_address, int port);

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
void chat_info();
void add_class(GtkWidget *widget, char *class_name);
void file_select(GtkWidget *widget, gpointer data);
void build_users();
void user_box(t_chat *chat, bool is_search);
bool check_field_size(const gchar *field, GtkWidget *field_widget);
void text_bubble(t_msg *message);
void delete_msg(GtkButton *__attribute__((unused)) button, gpointer data);
void delete_msg_id(GtkButton *__attribute__((unused)) button, gpointer msg_id);
void edit_msg(GtkButton *__attribute__((unused)) button, gpointer data);
void edit_accept(GtkButton *__attribute__((unused)) button, gpointer data);
void cancel_edit(GtkButton *__attribute__((unused)) button, gpointer data);
void change_msg_id_for_edit(GtkButton *__attribute__((unused)) button, gpointer msg_id);
GtkWidget *create_image_button(char *image_path, int width, int height);
void receive_bubble(t_msg *message);
void create_chat(GtkButton *__attribute__((unused)) button, GtkWidget *window);
void create_saved(GtkButton *__attribute__((unused)) button);
void join_chat(GtkWidget *__attribute__((unused)) widget, gpointer id);
void create_chat_menu();
void display_users();
char *get_send_time_str(time_t send_time);
void build_edit_profile();
void edit_username();
void accept_clicked(GtkButton *__attribute__((unused)) button, GtkWidget *window);
void last_massage_display(char *chatname, char *message);
char *str_to_display_last_msg(const char *msg, const char *username);
char *str_to_display_chat_name(char *chat_name);
void change_image (GtkWidget *button);
void delete_account(GtkWidget *__attribute__((unused)) button, GtkWidget *window);
bool check_str_for_spec_char(const char *str);
void empty_right_bar();
void empty_left_bar();
void close_window_by_button(GtkButton *button, gpointer *data);
void chat_menu(GtkWidget *__attribute__((unused)) button);
void chat_users(GtkWidget *window);
void chat_user_box(GtkWidget *window, t_user *user);
void chat_accept_clicked(GtkButton *__attribute__((unused)) button, GtkWidget *window);
void edit_chatname();

// CSS part
void load_css();
void hog();
