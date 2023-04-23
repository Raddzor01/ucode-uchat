#include "../../inc/server.h"

void join_chat(cJSON *chat_info, t_client_info *client_info)
{
    char *query = NULL;
    int chat_id;

    chat_id = cJSON_GetObjectItemCaseSensitive(chat_info, "chat_id")->valueint;

    if (db_check_chat_membership(chat_id, client_info->user->id))
    {
        send_responde(client_info->ssl, REQ_JOIN_CHAT, ERR_USER_IN_CHAT);
        return;
    }

    query = sqlite3_mprintf("INSERT INTO members (user_id, chat_id, privilege) VALUES(%d, %d, %d); ",
                            client_info->user->id, chat_id, PRIV_USER);
    db_execute_query(query);

    send_responde(client_info->ssl, REQ_JOIN_CHAT, ERR_SUCCESS);
    
    sqlite3_free(query);
}
