#include "../inc/client.h"

gboolean update_chatlist_from_thread(gpointer user_data)
{
    t_msg *server_msg = (t_msg *)user_data;
    if (server_msg->user_id == account->id)
        text_bubble(server_msg->text, server_msg->msg_id, server_msg->time);
    else
        receive_bubble(server_msg->text, server_msg->username);
    return FALSE;
}

void *server_update_thread()
{
    while (true)
    {
        if (!account)
            break;

        if (account && account->is_busy)
            continue;

        t_chat *chat = account ? account->chats : NULL;
        while (chat)
        {
            int last_server_msg_id = get_last_msg_id_from_server(chat->id);
            t_msg *last_msg = msg_get_last_message(chat->messages);
            int last_client_msg_id = last_msg ? last_msg->msg_id : 0;
            bool is_current = account->current_chat && account->current_chat->id == chat->id;
            if ((last_server_msg_id <= 0) || (last_server_msg_id <= last_client_msg_id && !is_current))
            {
                chat = chat->next;
                continue;
            }

            t_msg *server_msg = get_msg_by_id_from_server(last_server_msg_id, chat->id);
            if (last_server_msg_id > last_client_msg_id)
            {
                msg_push_back(&chat->messages, server_msg);
                if(is_current)
                    g_idle_add(update_chatlist_from_thread, (gpointer)server_msg);
                // if (server_msg->user_id == account->id)
                //     text_bubble(server_msg->text, server_msg->msg_id);
                // else
                //     receive_bubble(server_msg->text, server_msg->username);
                g_usleep(500000);
            }

            chat = chat->next;
            g_usleep(500000);
        }
    }

    return NULL;
}
