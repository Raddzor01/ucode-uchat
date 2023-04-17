#include "../inc/client.h"

int send_sign_up_to_server(const char *username, const char *password)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_SIGNUP);
    cJSON_AddStringToObject(json, "username", username);
    cJSON_AddStringToObject(json, "password", password);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return check_account_exists();
}

int send_login_to_server(const char *username, const char *password)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_LOGIN);
    cJSON_AddStringToObject(json, "username", username);
    cJSON_AddStringToObject(json, "password", password);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return check_account_from_server();
}

time_t send_message_to_server(const char *message)
{
    time_t current_time = time(NULL);

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_SEND_MSG);
    cJSON_AddNumberToObject(json, "user_id", account->id);
    cJSON_AddNumberToObject(json, "chat_id", account->chat_id);
    cJSON_AddStringToObject(json, "message", message);
    cJSON_AddStringToObject(json, "username", account->username);
    cJSON_AddNumberToObject(json, "time", current_time);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return current_time;
}

int send_exit_to_server()
{
    if (account->username != NULL)
        send_logout_to_server();

    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_CLIENT_EXIT);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return 0;
}

int send_logout_to_server()
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_USER_LOGOUT);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);

    mx_strdel(&account->username);
    chat_clear_list(&account->chats);

    return 0;
}

char *read_from_server()
{
    char buffer[BUFSIZ];
    int bytes = SSL_read(info->ssl, buffer, sizeof(buffer));

    if (bytes > 0)
    {
        buffer[bytes] = 0;
        return mx_strdup(buffer);
    }

    return NULL;
}

void read_from_server_to_logs()
{
    char *json_str = read_from_server();

    mx_logs(json_str, INFO_LOG);

    free(json_str);
}

t_msg *get_chat_messages_from_server(int chat_id)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_GET_CHAT_MESSAGES);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    cJSON *messages_arr = cJSON_GetObjectItemCaseSensitive(json, "messages");
    t_msg *msgs = NULL;

    cJSON *temp_json = NULL;
    for (int i = 0; i < cJSON_GetArraySize(messages_arr); i++)
    {
        temp_json = cJSON_GetArrayItem(messages_arr, i);

        t_msg *temp = (t_msg *)malloc(sizeof(t_msg));
        temp->text = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "message")->valuestring);
        temp->msg_id = cJSON_GetObjectItem(temp_json, "message_id")->valueint;
        temp->user_id = cJSON_GetObjectItem(temp_json, "user_id")->valueint;
        temp->username = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "username")->valuestring);
        temp->time = cJSON_GetObjectItemCaseSensitive(temp_json, "date")->valueint;
        temp->next = NULL;

        msg_push_back(&msgs, temp);
    }

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return msgs;
}

int get_user_chats()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_CHATS);

    char *json_str = cJSON_PrintUnformatted(json);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    cJSON *chats_array = cJSON_Parse(json_str);
    json = cJSON_GetObjectItemCaseSensitive(chats_array, "chats");

    // chat_clear_list(&account->chats);

    pthread_mutex_lock(&account->mutex);
    cJSON *temp_json = NULL;
    for (int i = 0; i < cJSON_GetArraySize(json); i++)
    {
        temp_json = cJSON_GetArrayItem(json, i);

        t_chat *new_node = (t_chat *)malloc(sizeof(t_chat));
        new_node->name = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "chat_name")->valuestring);
        new_node->id = cJSON_GetObjectItem(temp_json, "chat_id")->valueint;
        new_node->messages = get_chat_messages_from_server(new_node->id);
        new_node->next = NULL;
        chat_push_back(&account->chats, new_node);
    }
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return 0;
}

int get_msg_id()
{
    char *json_str = read_from_server();
    cJSON *json = cJSON_Parse(json_str);
    int msg_id;

    if (json == NULL)
    {
        mx_printerr("Error receiving id message\ncJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    msg_id = cJSON_GetObjectItem(json, "message_id")->valueint;

    cJSON_Delete(json);
    free(json_str);

    return msg_id;
}

t_chat *find_chats_from_server(const char *search_pattern)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_SEARCH_CHATS);
    cJSON_AddStringToObject(json, "search_pattern", search_pattern);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    cJSON *json_2 = cJSON_GetObjectItemCaseSensitive(json, "chats");
    t_chat *chats = NULL;
    cJSON *temp_json = NULL;
    for (int i = 0; i < cJSON_GetArraySize(json_2); i++)
    {
        temp_json = cJSON_GetArrayItem(json_2, i);

        t_chat *new_node = (t_chat *)malloc(sizeof(t_chat));
        new_node->name = mx_strdup(cJSON_GetObjectItemCaseSensitive(temp_json, "chat_name")->valuestring);
        new_node->id = cJSON_GetObjectItem(temp_json, "chat_id")->valueint;
        new_node->messages = get_chat_messages_from_server(new_node->id);
        new_node->next = NULL;
        chat_push_back(&chats, new_node);
    }

    cJSON_Delete(json);
    mx_strdel(&json_str);

    return chats;
}

int create_chat_in_server(const char *chat_name, int chat_type)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_CREATE_CHAT);
    cJSON_AddStringToObject(json, "name", chat_name);
    cJSON_AddNumberToObject(json, "chat_type", chat_type);
    cJSON_AddNumberToObject(json, "time", time(NULL));
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return 0;
}

int check_account_exists()
{
    char *json_str = read_from_server();
    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        mx_printerr("Error getting user information for sign up\ncJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
    // t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

    // if (type == REQ_USER_SIGNUP) {
    // 	mx_printerr("Error in type\n");
    // 	return NULL;
    // }

    cJSON_Delete(json);
    free(json_str);

    return error;
}

bool check_account_from_server()
{
    char *json_str = read_from_server();
    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        mx_printerr("Error getting user information for login\ncJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return 0;
    }

    t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
    // t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

    // add error checks
    if (error != 0)
    {
        mx_printerr("Error getting user information for login\nError in account cJSON\n");
        cJSON_Delete(json);
        free(json_str);
        return 0;
    }

    pthread_mutex_lock(&account->mutex);
    account->id = cJSON_GetObjectItem(json, "id")->valueint;
    account->username = mx_strdup(cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring);
    pthread_mutex_unlock(&account->mutex);
    // char *password = cJSON_GetObjectItem(json, "password")->valuestring;
    // printf("%d\t%s\n", account->id, account->username);
    cJSON_Delete(json);
    free(json_str);

    return 1;
}

int check_chat_id_from_server()
{
    char *json_str = read_from_server();
    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        mx_printerr("cJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

    cJSON_Delete(json);
    free(json_str);
    return chat_id;
}

int edit_msg_in_server(int msg_id, const char *new_text)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_EDIT_MESSAGE);
    cJSON_AddNumberToObject(json, "id", msg_id);
    cJSON_AddNumberToObject(json, "chat_id", account->chat_id);
    cJSON_AddStringToObject(json, "text", new_text);

    char *json_str = cJSON_PrintUnformatted(json);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    t_msg *edited_msg = msg_get_by_id(account->current_chat->messages, msg_id);
    mx_strdel(&edited_msg->text);

    pthread_mutex_lock(&account->mutex);
    edited_msg->text = mx_strdup(new_text);
    pthread_mutex_unlock(&account->mutex);

    cJSON_Delete(json);
    free(json_str);

    return 0;
}

int delete_msg_in_server(int msg_id)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_DEL_MESSAGE);
    cJSON_AddNumberToObject(json, "chat_id", account->chat_id);
    cJSON_AddNumberToObject(json, "message_id", msg_id);

    char *json_str = cJSON_PrintUnformatted(json);
    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    mx_strdel(&json_str);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    if (cJSON_GetObjectItem(json, "error_code")->valueint != 0)
        printf("Error deleting message\n");

    pthread_mutex_lock(&account->mutex);
    msg_pop_id(&account->current_chat->messages, msg_id);
    pthread_mutex_unlock(&account->mutex);

    mx_strdel(&json_str);
    cJSON_Delete(json);

    return 0;
}

int get_last_msg_id_from_server(int chat_id)
{
    int msg_id = 0;

    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_LAST_MSG_ID);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    int error_code = cJSON_GetObjectItem(json, "error_code")->valueint;
    if (error_code != 0)
    {
        printf("Error getting last message, error code - %d\n", error_code);
        return -1;
    }

    msg_id = cJSON_GetObjectItem(json, "message_id")->valueint;

    mx_strdel(&json_str);
    cJSON_Delete(json);

    return msg_id;
}

t_msg *get_msg_by_id_from_server(int msg_id, int chat_id)
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "type", REQ_GET_MSG_BY_ID);
    cJSON_AddNumberToObject(json, "message_id", msg_id);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    mx_strdel(&json_str);
    cJSON_Delete(json);

    json_str = read_from_server();
    json = cJSON_Parse(json_str);

    if (cJSON_GetObjectItem(json, "error_code")->valueint != ERR_SUCCESS)
        return NULL;

    cJSON *message_json = cJSON_GetObjectItem(json, "message");

    t_msg *new_node = (t_msg *)malloc(sizeof(t_msg));
    new_node->text = mx_strdup(cJSON_GetObjectItemCaseSensitive(message_json, "message")->valuestring);
    new_node->msg_id = cJSON_GetObjectItem(message_json, "message_id")->valueint;
    new_node->user_id = cJSON_GetObjectItem(message_json, "user_id")->valueint;
    new_node->username = mx_strdup(cJSON_GetObjectItemCaseSensitive(message_json, "username")->valuestring);
    new_node->time = cJSON_GetObjectItemCaseSensitive(message_json, "date")->valueint;
    new_node->next = NULL;

    return new_node;
}

int join_to_found_chat(int chat_id)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "type", REQ_JOIN_CHAT);
    cJSON_AddNumberToObject(json, "chat_id", chat_id);
    char *json_str = cJSON_PrintUnformatted(json);

    SSL_write(info->ssl, json_str, mx_strlen(json_str));

    cJSON_Delete(json);
    free(json_str);

    return 0;
}
