#include <time.h>

#define LOG_ERROR 1
#define LOG_INFO 0

// message list structure, functions starting with chat_
typedef struct s_msg
{
    int msg_id;
    int user_id;
    int image_id;
    char *text;
    char *username;
    time_t time;

    struct s_msg *next;
} t_msg;

typedef struct s_user
{
    int id;
    char *username;
    int image_id;
    int privilege;
}   t_user;

// chat list structure, functions starting with msg_
typedef struct s_chat
{
    int id;
    char *name;
    int image_id;
    char *image_path;
    int chat_type;
    int user_privilege;

    t_user **users;
    int users_count;

    t_msg *messages;
    struct s_chat *next;
} t_chat;

// enumeration of server request types
typedef enum e_req_type
{
    REQ_SERVER_ONLINE,
    REQ_USER_SIGNUP,
    REQ_USER_LOGIN,
    REQ_SEND_MSG,
    REQ_CHECK_FILE,
    REQ_UPDATE_CHAT_PHOTO,
    REQ_SEND_FILE_TO_SERVER,
    REQ_SEND_FILE_TO_CLIENT,
    REQ_CREATE_CHAT,
    REQ_GET_CHATS,
    REQ_GET_CHAT_MESSAGES,
    REQ_GET_MSG_BY_ID,
    REQ_GET_LAST_MSG_ID,
    REQ_SEARCH_CHATS,
    REQ_EDIT_MESSAGE,
    REQ_DEL_MESSAGE,
    REQ_DEL_CHAT,
    REQ_EDIT_CHAT_NAME,
    REQ_JOIN_CHAT,
    REQ_LEAVE_CHAT,
    REQ_KICK_USER,
    REQ_GET_CHAT_USERS,
    REQ_EDIT_USERNAME,
    REQ_EDIT_PASSWORD,
    REQ_DEL_ACCOUNT,
    REQ_USER_LOGOUT,
    REQ_CLIENT_EXIT,
    REQ_UNKNOWN
}   t_req_type;

// enumeration of error types
typedef enum e_error_type
{
    ERR_SUCCESS,
    ERR_JSON,
    ERR_INVALID_PASSWORD,
    ERR_USER_EXISTS,
    ERR_USER_NONEXIST,
    ERR_CHAT_EXIST,
    ERR_CHAT_NONEXIST,
    ERR_USER_IN_CHAT,
    ERR_USER_NOT_IN_CHAT,
    ERR_USER_DONT_HAVE_PERMISSION
} t_error_type;

// enumeration of user privilege
typedef enum e_privilege_type
{
    PRIV_USER,
    PRIV_MODERATOR,
    PRIV_ADMIN
}   t_privilege_type;

// enumaration of chat types
typedef enum e_chat_type
{
    CHAT_NORMAL,
    CHAT_PRIVATE,
    CHAT_SAVED
}   t_chat_type;

typedef enum e_pfp_type
{
    PFP_CHAT,
    PFP_USER,
    PFP_SAVED
}   t_pfp_type;

// message list functions
t_msg *msg_prepare_node(const int id, const char *message, const time_t time, const int user_id, const char *username);
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

// chat list functions
t_chat *chat_prepare_node(const int id, const char *name, const int image_id, const int user_privilege, int chat_type);
void chat_push_back(t_chat **list, t_chat *new_node);
void chat_push_front(t_chat **list, t_chat *new_node);
t_chat *chat_get_chat_by_id(t_chat *list, int chat_id);
t_chat *chat_get_chat_by_type(t_chat *list, int type);
void chat_clear_single_node(t_chat **list);
void chat_clear_list(t_chat **list);
int chat_list_size(t_chat *list);
void chat_move_node_to_head(t_chat **head, int node_id);
void chat_pop_by_id(t_chat **head, int id);
void chat_pop_index(t_chat **list, int id);
void chat_pop_front(t_chat **head);
void chat_pop_back(t_chat **head);
void pop_back_chat(t_chat **head);
