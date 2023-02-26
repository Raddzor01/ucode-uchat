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

    hog();

    if (main_window)
        clear_window(main_window);

    build_chat_window(info);

    gtk_widget_show_all(main_window);
    
}
