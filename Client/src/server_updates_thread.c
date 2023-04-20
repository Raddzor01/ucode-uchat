#include "../inc/client.h"

gboolean update_chatlist_from_thread(gpointer user_data)
{
    t_msg *server_message = (t_msg *)user_data;
    if (server_message->user_id != account->id)
        receive_bubble(server_message);
    return FALSE;
}

void *server_update_thread()
{
    char *last_msg_str;
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

            t_msg *server_message = get_msg_by_id_from_server(last_server_msg_id, chat->id);
            if (last_server_msg_id > last_client_msg_id)
            {
                pthread_mutex_lock(&account->mutex);
                msg_push_back(&chat->messages, server_message);
                pthread_mutex_unlock(&account->mutex);
                
                if(is_current)
                    g_idle_add(update_chatlist_from_thread, (gpointer)server_message);
                // if (server_message->user_id == account->id)
                //     text_bubble(server_message->text, server_message->msg_id);
                // else
                //     receive_bubble(server_message->text, server_message->username);
                // if (chat->id != account->current_chat->id)
                last_msg_str = str_to_display_last_msg(server_message->text, server_message->username);
                last_massage_display(chat->name, last_msg_str);
                g_usleep(10000);
            }
            g_usleep(500000);
            chat = chat->next;
        }
        g_usleep(500000);
    }
    free(last_msg_str);

    return NULL;
}
