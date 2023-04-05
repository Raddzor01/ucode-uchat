#include "../inc/client.h"

void send_message(GtkButton *button) {

    const char *text;

    text = gtk_entry_get_text (GTK_ENTRY (info->entry));

    text_bubble(text);

    send_message_to_server(text);
    // create_chat_in_server(text, CHAT_NORMAL);

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

    find_chats_from_server(text);

    // gtk_entry_set_text(GTK_ENTRY(entry), "");

    read_found_chats();
}

void chat_window() {

    hog();

    get_user_chats();
    // send_from_server();

    if (main_window)
        clear_window(main_window);
    
    create_new_window("UChat", 500, 400, true);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(main_window), grid);
    build_users(grid);

    build_chat_window(grid);

    gtk_widget_show_all(main_window);
}
