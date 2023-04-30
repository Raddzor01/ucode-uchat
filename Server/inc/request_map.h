#include "server.h"

#define MAP_SIZE 27 // request map size

void user_signup(cJSON *json, t_client_info *client_info);
void user_login(cJSON *json, t_client_info *client_info);
void send_message(cJSON *json, t_client_info *client_info);
void update_user_photo(cJSON *json, t_client_info *client_info);
void create_chat(cJSON *json, t_client_info *client_info);
void get_chats(cJSON *__attribute__((unused)) json, t_client_info *client_info);
void get_chat_messages(cJSON *json, t_client_info *client_info);
void get_last_message_id(cJSON *json, t_client_info *client_info);
void get_message_by_id(cJSON *json, t_client_info *client_info);
void search_chats(cJSON *client_json, t_client_info *client_info);
void edit_message(cJSON *client_json, t_client_info *client_info);
void delete_message(cJSON *json, t_client_info *client_info);
void join_chat(cJSON *chat_info, t_client_info *client_info);
void edit_username(cJSON *json, t_client_info *client_info);
void edit_password(cJSON *json, t_client_info *client_info);
void user_logout(cJSON *__attribute__((unused)) json, t_client_info *client_info);
void client_exit(cJSON *__attribute__((unused)) json, t_client_info *client_info);
void leave_chat(cJSON *json, t_client_info *client_info);
void delete_account(cJSON *__attribute__((unused)) json, t_client_info *client_info);
void delete_chat(cJSON *json, t_client_info *client_info);
void send_image(cJSON *json, t_client_info *client_info);
void server_online(cJSON *json, t_client_info *client_info);
void check_file(cJSON *json, t_client_info *client_info);
void update_chat_photo(cJSON *json, t_client_info *client_info);
void get_chat_users(cJSON *json, t_client_info *client_info);
void edit_chat_name(cJSON *json, t_client_info *client_info);
void kick_user(cJSON *json, t_client_info *client_info);

typedef void (*request_handler)(cJSON *, t_client_info *);

typedef struct
{
    t_req_type type;
    request_handler handler;
} t_map_entry;

static t_map_entry request_map[MAP_SIZE] = 
{
    {REQ_SERVER_ONLINE, server_online},
    {REQ_USER_SIGNUP, user_signup},
    {REQ_USER_LOGIN, user_login},
    {REQ_SEND_MSG, send_message},
    {REQ_CHECK_FILE, check_file},
    {REQ_UPDATE_CHAT_PHOTO, update_chat_photo},
    {REQ_SEND_FILE_TO_SERVER, update_user_photo},
    {REQ_SEND_FILE_TO_CLIENT, send_image},
    {REQ_CREATE_CHAT, create_chat},
    {REQ_GET_CHATS, get_chats},
    {REQ_GET_CHAT_MESSAGES, get_chat_messages},
    {REQ_GET_MSG_BY_ID, get_message_by_id},
    {REQ_GET_LAST_MSG_ID, get_last_message_id},
    {REQ_SEARCH_CHATS, search_chats},
    {REQ_EDIT_MESSAGE, edit_message},
    {REQ_DEL_MESSAGE, delete_message},
    {REQ_DEL_CHAT, delete_chat},
    {REQ_EDIT_CHAT_NAME, edit_chat_name},
    {REQ_JOIN_CHAT, join_chat},
    {REQ_LEAVE_CHAT, leave_chat},
    {REQ_KICK_USER, kick_user},
    {REQ_GET_CHAT_USERS, get_chat_users},
    {REQ_EDIT_USERNAME, edit_username},
    {REQ_EDIT_PASSWORD, edit_password},
    {REQ_DEL_ACCOUNT, delete_account},
    {REQ_USER_LOGOUT, user_logout},
    {REQ_CLIENT_EXIT, client_exit}
};
