#include "../../inc/server.h"

void leave_chat(cJSON *json, t_client_info *client_info)
{
    int chat_id;
    char *query = NULL;

    chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;

    if(!db_check_chat_exists(chat_id))
    {
        send_responde(client_info->ssl, REQ_LEAVE_CHAT, ERR_CHAT_NONEXIST);
        return;
    }
    if(!db_check_chat_membership(chat_id, client_info->user->id))
    {
        send_responde(client_info->ssl, REQ_LEAVE_CHAT, ERR_USER_NOT_IN_CHAT);
        return;
    }

    query = sqlite3_mprintf("DELETE FROM members WHERE chat_id = %d AND user_id = %d; ",
                            chat_id, client_info->user->id);
    db_execute_query(query);
    
    send_responde(client_info->ssl, REQ_LEAVE_CHAT, ERR_SUCCESS);

    mx_strdel(&query);
}
