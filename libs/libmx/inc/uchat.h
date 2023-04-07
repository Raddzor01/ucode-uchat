typedef enum e_req_type
{
    REQ_USER_SIGNUP,
    REQ_USER_LOGIN,
    REQ_SEND_MSG,
    REQ_SEND_FILE,
    REQ_CREATE_CHAT,
    REQ_GET_CHATS,
    REQ_GET_CHAT_MESSAGES,
    REQ_SEARCH_CHATS,
    REQ_EDIT_MESSAGE,
    REQ_DEL_MESSAGE,
    REQ_JOIN_CHAT,
    REQ_EDIT_USERNAME,
    REQ_EDIT_PASSWORD,
    REQ_UNKNOWN,
    REQ_LOGOUT,
    REQ_EXIT,
}   t_req_type;

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
    ERR_USER_NOT_IN_CHAT
} t_error_type;

typedef enum e_chat_type {
    CHAT_NORMAL,
    CHAT_PRIVATE
}   t_chat_type;

typedef enum e_user_type {
    USERTYPE_NORMAL,
    USERTYPE_ADMIN
}   t_user_type;
