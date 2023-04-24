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
    empty_left_bar();
    empty_right_bar();

    gtk_widget_show_all(main_window);
    return FALSE;
}

gboolean update_chatbox_from_thread(gpointer __attribute__((unused)) data)
{
    GtkWidget *chat = get_widget_by_name_r(main_window, "chat");
    GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");

    gtk_widget_destroy(chat);
    clear_box(box);

    display_users();
    empty_left_bar();
    empty_right_bar();

    gtk_widget_show_all(main_window);
    return FALSE;
}

void delete_chat_from_thread(int id)
{
    pthread_mutex_lock(&account->mutex);
    chat_pop_by_id(&account->chats, id);
    pthread_mutex_unlock(&account->mutex);
    g_idle_add(update_chatbox_from_thread, NULL);
    g_usleep(10000);
}

void update_message(t_msg *server_message_node, t_chat *chat, bool is_current)
{
    char *last_message_str = NULL;
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

void *server_update_thread()
{
    int last_server_msg_id = 0;
    int last_client_msg_id = 0;
    t_chat *chat = NULL;
    t_msg *last_message_node = NULL;
    t_msg *server_message_node = NULL;
    bool is_current = false;

    while (true)
    {
        chat = account ? account->chats : NULL;
        while (chat)
        {
            last_server_msg_id = chat ? get_last_msg_id_from_server(chat->id) : 0;
            if (last_server_msg_id == -2)
            {
                delete_chat_from_thread(chat->id);
                if (chat_list_size(account->chats) == 0)
                    chat = NULL;
                else
                    chat = chat ? chat->next : NULL;
                continue;
            }
            last_message_node = chat ? msg_get_last_message(chat->messages) : NULL;
            last_client_msg_id = last_message_node ? last_message_node->msg_id : 0;
            is_current = account->current_chat && account->current_chat->id == chat->id;
            if ((last_server_msg_id <= 0) || (last_server_msg_id <= last_client_msg_id && !is_current))
            {
                chat = chat ? chat->next : NULL;
                continue;
            }

            server_message_node = get_msg_by_id_from_server(last_server_msg_id, chat->id);
            if (last_server_msg_id > last_client_msg_id)
                update_message(server_message_node, chat, is_current);

            g_usleep(500000);
            chat = chat ? chat->next : NULL;
        }
        g_usleep(500000);
    }

    return NULL;
}
