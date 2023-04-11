#include "server.h"

#define MAP_SIZE 15 // request map size

void user_signup(cJSON *json, t_client_info *client_info);
void user_login(cJSON *json, t_client_info *client_info);
void send_message(cJSON *json, t_client_info *client_info);
void get_user_file(cJSON *json, t_client_info *client_info);
void create_chat(cJSON *json, t_client_info *client_info);
void get_chats(cJSON *__attribute__((unused)) json, t_client_info *client_info);
void get_chat_messages(cJSON *json, t_client_info *client_info);
void search_chats(cJSON *client_json, t_client_info *client_info);
void edit_message(cJSON *client_json, t_client_info *client_info);
void delete_message(cJSON *json, t_client_info *client_info);
void join_chat(cJSON *chat_info, t_client_info *client_info);
void edit_username(cJSON *json, t_client_info *client_info);
void edit_password(cJSON *json, t_client_info *client_info);
void user_logout(cJSON *__attribute__((unused)) json, t_client_info *client_info);
void client_exit(cJSON *__attribute__((unused)) json, t_client_info *client_info);

typedef void (*request_handler)(cJSON *, t_client_info *);

typedef struct
{
    t_req_type type;
    request_handler handler;
} t_map_entry;

static t_map_entry request_map[MAP_SIZE] = 
{
    {REQ_USER_SIGNUP, user_signup},
    {REQ_USER_LOGIN, user_login},
    {REQ_SEND_MSG, send_message},
    {REQ_SEND_FILE, get_user_file},
    {REQ_CREATE_CHAT, create_chat},
    {REQ_GET_CHATS, get_chats},
    {REQ_GET_CHAT_MESSAGES, get_chat_messages},
    {REQ_SEARCH_CHATS, search_chats},
    {REQ_EDIT_MESSAGE, edit_message},
    {REQ_DEL_MESSAGE, delete_message},
    {REQ_JOIN_CHAT, join_chat},
    {REQ_EDIT_USERNAME, edit_username},
    {REQ_EDIT_PASSWORD, edit_password},
    {REQ_USER_LOGOUT, user_logout},
    {REQ_CLIENT_EXIT, client_exit}
};
