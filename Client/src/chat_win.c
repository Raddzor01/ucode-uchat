#include "../inc/client.h"

void send_message(GtkButton *button) {

    const char *text;

    text = gtk_entry_get_text(GTK_ENTRY(info->entry));

    if(strlen(text) == 0)
        return;

    time_t timer = send_message_to_server(text);
    // create_chat_in_server(text, CHAT_NORMAL);
    timer++;

    text_bubble(text, get_msg_id());
    // receive_bubble(text, "hog");
    // receive_bubble(text, "hog");
    // receive_bubble(text, "hog");

    // if (strcmp(text, "e") == 0)
    //     account->chat_id = 2;

    gtk_entry_set_text(GTK_ENTRY(info->entry), "");

    if(info){}
        (void)button;
}

void find_chats(GtkWidget *entry) {

    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));

    if (mx_strlen(text) <= 0) 
        return;

    GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");
    clear_box(box);

    if (strlen(gtk_entry_get_text(GTK_ENTRY(entry))) == 0) {
        display_users();
        return;
    }

    t_chat **chat = find_chats_from_server(text);

    for (int i = 0; chat[i] != NULL; i++) 
        user_box(chat[i]->chat_name, chat[i]->chat_id);

    // read_from_server_to_logs();
}

void chat_window() {

    hog();

    get_user_chats();

    if (main_window)
        clear_window(main_window);
    
    create_new_window("UChat", 500, 400, true);

    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_name(grid, "chat_grid");
    gtk_container_add(GTK_CONTAINER(main_window), grid);
    build_users(grid);

    build_chat_window();

    gtk_widget_show_all(main_window);
}
