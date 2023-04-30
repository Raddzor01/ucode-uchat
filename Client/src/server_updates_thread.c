#include "../inc/client.h"

extern GtkWidget *edit_window;

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
    if (GTK_IS_WINDOW(edit_window))
        gtk_widget_destroy(edit_window);
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

gboolean delete_message_from_thread_by_id(gpointer data)
{
    int msg_id = GPOINTER_TO_INT(data);
    char *msg_name = mx_strjoin(MSG_NAME, mx_itoa(msg_id));
    GtkWidget *msg = get_widget_by_name_r(main_window, msg_name);

    gtk_widget_destroy(msg);

    gtk_widget_show_all(main_window);

    mx_strdel(&msg_name);
    return FALSE;
}

gboolean edit_message_from_thread_by_id(gpointer data)
{
    t_msg *msg = (t_msg *)data;
    char *msg_name = mx_strjoin(MSG_NAME, mx_itoa(msg->msg_id));
    GtkWidget *msg_box = get_widget_by_name_r(main_window, msg_name);
    GtkWidget *msg_text_view = get_widget_by_name_r(msg_box, "message_text_view");
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(msg_text_view));

    PangoLayout *layout = gtk_widget_create_pango_layout(msg_text_view, msg->text);
    int width, height;
    pango_layout_get_pixel_size(layout, &width, &height);

    if (width > 400)
    {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(msg_text_view), GTK_WRAP_WORD_CHAR);
        gtk_widget_set_size_request(msg_text_view, 400, -1);
        gtk_widget_set_halign(msg_text_view, GTK_ALIGN_END);
    }
    else
    {
        gtk_widget_set_size_request(msg_text_view, width, -1);
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(msg_text_view), GTK_WRAP_NONE);
        gtk_widget_set_halign(msg_text_view, GTK_ALIGN_START);
    }

    g_object_unref(layout);
    gtk_text_buffer_set_text(buffer, msg->text, strlen(msg->text));

    gtk_widget_show_all(main_window);

    mx_strdel(&msg_name);
    return FALSE;
}

void delete_message_from_thread(t_chat *chat, int msg_id, bool is_current)
{
    if (is_current)
        g_idle_add(delete_message_from_thread_by_id, GINT_TO_POINTER(msg_id));
    pthread_mutex_lock(&account->mutex);
    msg_pop_by_message_id(&chat->messages, msg_id);
    pthread_mutex_unlock(&account->mutex);
    g_usleep(10000);
}

void update_chat_users_from_thread(t_chat *chat, t_user **server_users, int users_count, bool is_current)
{
    pthread_mutex_lock(&account->mutex);
    for (int i = 0; chat->users[i] != NULL; i++)
    {
        free(chat->users[i]);
        chat->users[i] = NULL;
    }
    free(chat->users);
    chat->users = NULL;
    chat->users = server_users;
    chat->users_count = users_count;
    pthread_mutex_unlock(&account->mutex);

    if (is_current)
        g_idle_add(update_chatbox_from_thread, NULL);
}

void edit_message_from_thread(t_chat *chat, t_msg *server_msg, bool is_current)
{
    if (is_current)
        g_idle_add(edit_message_from_thread_by_id, (gpointer)server_msg);
    pthread_mutex_lock(&account->mutex);
    msg_get_by_id(chat->messages, server_msg->msg_id)->text = server_msg->text;
    pthread_mutex_unlock(&account->mutex);
    g_usleep(10000);
}

void update_message(t_msg *server_message_node, t_chat *chat, bool is_current)
{
    char *last_message_str = NULL;
    if (chat->id == account->chats->id)
    {
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

    t_msg *new_node = msg_prepare_node(server_message_node->msg_id, server_message_node->text, server_message_node->time, server_message_node->user_id, server_message_node->username);
    new_node->image_id = server_message_node->image_id;
    pthread_mutex_lock(&account->mutex);
    msg_push_back(&chat->messages, new_node);
    chat_move_node_to_head(&account->chats, chat->id);
    pthread_mutex_unlock(&account->mutex);

    g_usleep(10000);
}

void *server_update_thread()
{
    int last_server_msg_id = 0;
    t_chat *chat = NULL;
    bool is_current = false;
    t_msg *server_messages = NULL;
    int server_messages_size = 0;
    int client_messages_size = 0;
    t_msg *msg = NULL;
    t_msg *server_msg = NULL;

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
            server_messages = chat ? get_chat_messages_from_server(chat->id) : NULL;
            server_messages_size = msg_list_size(server_messages);
            client_messages_size = msg_list_size(chat->messages);
            is_current = account->current_chat && account->current_chat->id == chat->id;

            msg = chat ? chat->messages : NULL;
            server_msg = server_messages;
            while (server_msg)
            {
                if (server_msg && !msg && (client_messages_size < server_messages_size))
                {
                    update_message(server_msg, chat, is_current);
                    client_messages_size++;
                    msg = chat ? msg_get_last_message(chat->messages)->next : NULL;
                    server_msg = server_msg->next;
                    continue;
                }

                if (server_msg->msg_id != msg->msg_id)
                {
                    delete_message_from_thread(chat, msg->msg_id, is_current);
                    client_messages_size--;
                    msg = chat ? msg->next : NULL;
                    continue;
                }

                if (strcmp(server_messages ? server_msg->text : msg->text, msg->text) != 0)
                    edit_message_from_thread(chat, server_msg, is_current);

                msg = chat ? msg->next : NULL;
                server_msg = server_msg->next;
            }

            if (msg)
            {
                delete_message_from_thread(chat, msg->msg_id, is_current);
                msg = chat ? msg->next : NULL;
            }

            msg_clear_list(&server_messages);

            bool is_chat_users_changed = false;
            t_user **server_chat_users = get_chat_users_from_server(chat->id);
            int users_count;
            for (users_count = 0; server_chat_users[users_count] != NULL; users_count++)
            {
                if (server_chat_users[users_count] && !chat->users[users_count] && users_count > chat->users_count - 1)
                {
                    is_chat_users_changed = true;
                    break;
                }
            }

            if(users_count + 1 < chat->users_count)
                is_chat_users_changed = true;

            if (is_chat_users_changed)
                update_chat_users_from_thread(chat, server_chat_users, users_count + 1, is_current);
            else
            {
                for (int i = 0; server_chat_users[i] != NULL; i++)
                {
                    free(server_chat_users[i]);
                    server_chat_users[i] = NULL;
                }
                free(server_chat_users);
                server_chat_users = NULL;
            }

            g_usleep(500000);
            chat = chat ? chat->next : NULL;
        }
        g_usleep(500000);
    }

    return NULL;
}
