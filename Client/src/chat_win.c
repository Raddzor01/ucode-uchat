#include "../inc/client.h"

void send_message(GtkButton *__attribute__((unused)) button)
{
    time_t timer = 0;
    int server_msg_id = 0;
    char *text = NULL;
    t_msg *new_node = NULL;

    text = (char *)gtk_entry_get_text(GTK_ENTRY(info->entry));

    if (mx_strlen(text) == 0)
        return;

    timer = send_message_to_server(text);
    server_msg_id = get_msg_id();

    new_node = msg_prepare_node(server_msg_id, text, timer, account->id, account->username);

    text_bubble(new_node);

    gtk_entry_set_text(GTK_ENTRY(info->entry), "");

    pthread_mutex_lock(&account->mutex);
    msg_push_back(&account->current_chat->messages, new_node);
    pthread_mutex_unlock(&account->mutex);

    chat_move_node_to_head(&account->chats, account->current_chat->id);
    GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");
    clear_box(box);
    display_users();
}

void find_chats(GtkWidget *entry)
{
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));

    GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");
    clear_box(box);

    if (mx_strlen(text) <= 0)
    {
        display_users();
        return;
    }

    t_chat *chats = find_chats_from_server(text);
    while (chats)
    {
        user_box(chats, true);
        chats = chats->next;
    }
}

void chat_window()
{
    // hog();

    account->chats = get_user_chats();

    if (main_window)
        clear_window(main_window);
    create_new_window("UChat", 700, 400, true);

    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_name(grid, "chat_grid");
    gtk_container_add(GTK_CONTAINER(main_window), grid);
    build_users();
    empty_right_bar();

    // build_chat_window();

    pthread_create(&account->server_online_thread, NULL, server_online_check_thread, NULL);
    pthread_create(&account->server_update_thread, NULL, server_update_thread, NULL);

    gtk_widget_show_all(main_window);
}
