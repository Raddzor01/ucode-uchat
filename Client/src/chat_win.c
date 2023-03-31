#include "../inc/client.h"

void send_message(GtkButton *button) {

    const char *text;

    text = gtk_entry_get_text (GTK_ENTRY (info->entry));

    text_bubble(text);

    send_message_to_server(text);
    // create_chat_in_server(text, CHAT_NORMAL);

    // if (strcmp(text, "e") == 0)
    //     account->chat_id = 2;

    gtk_entry_set_text (GTK_ENTRY (info->entry), "");

    if(info){}
        (void)button;
}

void send_button_clicked(GtkButton *button) {

    info->text_view  = g_object_get_data(G_OBJECT(info->entry), "text_view");

    send_message(button);
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
