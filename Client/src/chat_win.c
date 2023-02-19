#include "../inc/client.h"

void send_message(t_info *info, GtkTextView *text_view) {
    GtkTextBuffer *buffer;
    GtkTextIter iter;
    const char *text;

    buffer = gtk_text_view_get_buffer (text_view);
    text = gtk_entry_get_text (GTK_ENTRY (info->entry));
    send_to_server(info->ssl, text);
    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_buffer_insert (buffer, &iter, text, -1);
    gtk_text_buffer_insert (buffer, &iter, "\n", -1);
    gtk_entry_set_text (GTK_ENTRY (info->entry), "");
    gtk_text_view_scroll_to_iter (text_view, &iter, 0.0, FALSE, 0.0, 0.0);
}

void send_button_clicked(GtkButton *button, t_info *info) {
    GtkTextView *text_view;

    text_view = g_object_get_data (G_OBJECT (info->entry), "text_view");
    send_message (info, text_view);
    if(button){}
        (void)info->entry;
}
