#include "../../inc/server.h"

void delete_message(cJSON *json, t_client_info *client_info)
{
    int chat_id;
    int message_id;
    char *query = NULL;

    chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    message_id = cJSON_GetObjectItem(json, "message_id")->valueint;

    if(!db_check_chat_exists(chat_id))
    {
        send_responde(client_info->ssl, REQ_DEL_MESSAGE, ERR_CHAT_NONEXIST);
        return;
    }

    if(!db_check_chat_membership(chat_id, client_info->user->id))
    {
        send_responde(client_info->ssl, REQ_DEL_MESSAGE, ERR_USER_NOT_IN_CHAT);
        return;
    }

    query = sqlite3_mprintf("DELETE FROM messages WHERE id = %d AND chat_id = %d; ", 
                            message_id, chat_id);
    db_execute_query(query);

    send_responde(client_info->ssl, REQ_DEL_MESSAGE, ERR_SUCCESS);
}
