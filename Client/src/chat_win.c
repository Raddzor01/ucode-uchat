#include "../inc/client.h"

void send_message(GtkButton *__attribute__((unused)) button)
{

    const char *text;

    text = gtk_entry_get_text(GTK_ENTRY(info->entry));

    if (strlen(text) == 0)
        return;

    time_t timer = send_message_to_server(text);
    // create_chat_in_server(text, CHAT_NORMAL);
    int server_msg_id = get_msg_id();

    t_msg *new_node = malloc(sizeof(t_msg));
    new_node->text = mx_strdup(text);
    new_node->time = timer;
    new_node->user_id = account->id;
    new_node->username = mx_strdup(account->username);
    new_node->next = NULL;

    pthread_mutex_lock(&account->mutex);
    msg_push_back(&account->current_chat->messages, new_node);
    pthread_mutex_unlock(&account->mutex);

    text_bubble(text, server_msg_id, timer);

    // if (strcmp(text, "e") == 0)
    //     account->chat_id = 2;

    gtk_entry_set_text(GTK_ENTRY(info->entry), "");
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
    hog();

    get_user_chats();

    if (main_window)
        clear_window(main_window);
    create_new_window("UChat", 700, 400, true);

    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_name(grid, "chat_grid");
    gtk_container_add(GTK_CONTAINER(main_window), grid);
    build_users(grid);

    // pthread_create(&account->server_update_thread, NULL, server_update_thread, NULL);
    
    // build_chat_window();

    gtk_widget_show_all(main_window);
}
