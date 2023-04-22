#include "../inc/client.h"

gboolean update_messages_from_thread(gpointer user_data)
{
    t_msg *server_message_node = (t_msg *)user_data;
    if (server_message_node->user_id != account->id)
        receive_bubble(server_message_node);
    return FALSE;
}

gboolean update_chatlist_from_thread(gpointer __attribute__((unused)) data)
{
    GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");
    clear_box(box);
    display_users();
    return FALSE;
}

void *server_update_thread()
{
    int last_server_msg_id = 0;
    int last_client_msg_id = 0;
    char *last_message_str = NULL;
    t_chat *chat = NULL;
    t_msg *last_message_node = NULL;
    t_msg *server_message_node = NULL;
    bool is_current = false;

    while (true)
    {
        if (!account)
            break;

        if (account && account->is_busy)
            continue;

        chat = account ? account->chats : NULL;
        while (chat)
        {
            last_server_msg_id = chat ? get_last_msg_id_from_server(chat->id) : 0;
            last_message_node = chat ? msg_get_last_message(chat->messages) : 0;
            last_client_msg_id = last_message_node ? last_message_node->msg_id : 0;
            is_current = account->current_chat && account->current_chat->id == chat->id;
            if ((last_server_msg_id <= 0) || (last_server_msg_id <= last_client_msg_id && !is_current))
            {
                chat = chat->next;
                continue;
            }

            server_message_node = get_msg_by_id_from_server(last_server_msg_id, chat->id);
            if (last_server_msg_id > last_client_msg_id)
            {
                if (chat->id == account->chats->id)
                {
                    // this will burn down gtk bruh, change it
                    last_message_str = str_to_display_last_msg(server_message_node->text, server_message_node->username);
                    last_massage_display(chat->name, last_message_str);
                    mx_strdel(&last_message_str);
                }
                else
                {
                    g_idle_add(update_chatlist_from_thread, NULL);
                }

                if (is_current)
                    g_idle_add(update_messages_from_thread, (gpointer)server_message_node);

                pthread_mutex_lock(&account->mutex);
                msg_push_back(&chat->messages, server_message_node);
                chat_move_node_to_head(&account->chats, chat->id);
                pthread_mutex_unlock(&account->mutex);

                g_usleep(10000);
            }

            g_usleep(500000);
            chat = chat->next;
        }
        g_usleep(500000);
    }

    return NULL;
}
