#include "../inc/client.h"

void send_message(GtkButton *button) {
    GtkTextBuffer *buffer;
    GtkTextIter iter;
    const char *text;

    buffer = gtk_text_view_get_buffer (info->text_view);
    text = gtk_entry_get_text (GTK_ENTRY (info->entry));
    // send_sign_up_to_server(info->ssl, text);
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
    // GtkTextView *text_view;

    info->text_view  = g_object_get_data(G_OBJECT(info->entry), "text_view");
    // info->text_view = text_view;
    send_message(button);
}

void chat_window() {

    hog();

    if (main_window)
        clear_window(main_window);
    create_new_window("UChat", 500, 400, true);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(main_window), grid);

    // gtk_widget_set_hexpand(grid, TRUE);
    // gtk_widget_set_vexpand(grid, TRUE);
    // gtk_widget_set_halign(grid, GTK_ALIGN_FILL);
    // gtk_widget_set_valign(grid, GTK_ALIGN_FILL);

    build_users(grid);

    build_chat_window(grid);

    gtk_widget_show_all(main_window);
    
}
