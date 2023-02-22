#include "../inc/client.h"

void send_message(GtkButton *button, t_info *info) {
    GtkTextBuffer *buffer;
    GtkTextIter iter;
    const char *text;

    buffer = gtk_text_view_get_buffer (info->text_view);
    text = gtk_entry_get_text (GTK_ENTRY (info->entry));
    send_to_server(info->ssl, text);
    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_buffer_insert (buffer, &iter, text, -1);
    gtk_text_buffer_insert (buffer, &iter, "\n", -1);
    gtk_entry_set_text (GTK_ENTRY (info->entry), "");
    gtk_text_view_scroll_to_iter (info->text_view, &iter, 0.0, FALSE, 0.0, 0.0);
    if(button){}
        (void)info->entry;
}

void send_button_clicked(GtkButton *button, t_info *info) {
    GtkTextView *text_view;

    text_view = g_object_get_data (G_OBJECT (info->entry), "text_view");
    info->text_view = text_view;
    send_message (button ,info);
}

void chat_window(t_info *info) {
    GtkWidget *box;
    GtkWidget *entry;
    GtkWidget *text_view;
    GtkWidget *send_button;
    GtkWidget *scrolled_window;

    // if (main_window)
    //     gtk_widget_destroy(main_window);
    
    // main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // gtk_window_set_title(GTK_WINDOW(main_window), "Chat");
    // gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 400);

    main_window = create_new_window("UChat", 500, 400, true);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(main_window), box);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    // gtk_box_pack_start (GTK_BOX (box), text_view, TRUE, TRUE, 0);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);
    g_object_set_data(G_OBJECT(entry), "text_view", text_view);
    info->text_view = GTK_TEXT_VIEW(text_view);
    g_signal_connect(entry, "activate", G_CALLBACK(send_message), info);

    send_button = gtk_button_new_with_label("Send");
    gtk_box_pack_start(GTK_BOX(box), send_button, FALSE, FALSE, 0);
    info->entry = entry;
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_button_clicked), info);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(main_window);
    
}
