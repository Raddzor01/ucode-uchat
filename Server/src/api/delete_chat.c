#include "../../inc/server.h"

void delete_chat(cJSON *json, t_client_info *client_info)
{
    char *query = NULL;

    int chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;

    if(!db_check_chat_membership(chat_id, client_info->user->id))
    {
        send_responde(client_info->ssl, REQ_DEL_CHAT, ERR_USER_NOT_IN_CHAT);
        return;
    }

    query = sqlite3_mprintf("DELETE FROM chats WHERE id = %d; ");
    db_execute_query(query);

    sqlite3_free(query);

    query = sqlite3_mprintf("DELETE FROM members WHERE chat_id = %d; ");
    db_execute_query(query);

    sqlite3_free(query);

    query = sqlite3_mprintf("DELETE FROM messages WHERE chat_id = %d; ");
    db_execute_query(query);

    sqlite3_free(query);

    send_responde(client_info->ssl, REQ_DEL_CHAT, ERR_SUCCESS);
}
