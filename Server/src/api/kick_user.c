#include "../../inc/server.h"

void kick_user(cJSON *json, t_client_info *client_info)
{
    char *query = NULL;
    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    int user_id = cJSON_GetObjectItem(json, "user_id")->valueint;

    if(!db_check_chat_membership(chat_id, client_info->user->id))
    {
        send_responde(client_info->ssl, REQ_KICK_USER, ERR_USER_NOT_IN_CHAT);
        return;
    }

    t_privilege_type user_priv = db_check_user_privilege(client_info->user->id, chat_id);
    if(user_priv == PRIV_USER)
    {
        send_responde(client_info->ssl, REQ_KICK_USER, ERR_USER_DONT_HAVE_PERMISSION);
        return;
    }

    query = sqlite3_mprintf("DELETE FROM members WHERE chat_id = %d AND user_id = %d; ",
                            chat_id, user_id);
    db_execute_query(query);

    sqlite3_free(query);

    send_responde(client_info->ssl, REQ_KICK_USER, ERR_SUCCESS);
}
