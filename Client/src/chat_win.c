#include "../inc/client.h"

void send_message(GtkButton *button) {

    GtkTextBuffer *buffer;
    GtkTextIter iter;
    const char *text;

    buffer = gtk_text_view_get_buffer (info->text_view);
    text = gtk_entry_get_text (GTK_ENTRY (info->entry));

    send_message_to_server(text);

    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_buffer_insert (buffer, &iter, text, -1);
    gtk_text_buffer_insert (buffer, &iter, "\n", -1);
    gtk_entry_set_text (GTK_ENTRY (info->entry), "");
    gtk_text_view_scroll_to_iter (info->text_view, &iter, 0.0, FALSE, 0.0, 0.0);

    if(info){}
        (void)button;
}

void send_button_clicked(GtkButton *button) {

    info->text_view  = g_object_get_data(G_OBJECT(info->entry), "text_view");

    send_message(button);
}

void chat_window() {

    hog();

    if (main_window)
        clear_window(main_window);
    
    create_new_window("UChat", 500, 400, true);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(main_window), grid);

    build_users(grid);

    build_chat_window(grid);

    gtk_widget_show_all(main_window);
}
