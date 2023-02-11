#include "../inc/client.h"

// static void activate (GtkApplication *app, gpointer user_data){
//     GtkWidget *window;

//     window = gtk_application_window_new(app);
//     gtk_window_set_resizable (window, gboolean resizable);

//     gtk_widget_show(window);
// }

// int main (int argc, char **argv) {
//     GtkApplication *app;
//     int status;

//     app = gtk_application_new("chat.ucode", G_APPLICATION_DEFAULT_FLAGS);
//     g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
//     status = g_application_run(G_APPLICATION(app), argc, argv);
//     g_object_unref(app);

//     return status;
// }

static void send_button_clicked (GtkButton *button, gpointer user_data) {
  GtkWidget *entry;
  GtkWidget *text_view;
  GtkTextBuffer *buffer;
  GtkTextIter end_iter;
  const gchar *text;

  entry = GTK_WIDGET (user_data);
  text_view = g_object_get_data (G_OBJECT (entry), "text_view");
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

  text = gtk_entry_get_text (GTK_ENTRY (entry));
  gtk_text_buffer_get_end_iter (buffer, &end_iter);
  gtk_text_buffer_insert (buffer, &end_iter, text, -1);
  gtk_text_buffer_insert (buffer, &end_iter, "\n", -1);
  gtk_entry_set_text (GTK_ENTRY (entry), "");
}

int main (int argc, char **argv) {
  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *entry;
  GtkWidget *text_view;
  GtkWidget *send_button;
  GtkTextBuffer *buffer;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Chat");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);

  box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add (GTK_CONTAINER (window), box);

  text_view = gtk_text_view_new ();
  gtk_box_pack_start (GTK_BOX (box), text_view, TRUE, TRUE, 0);
  gtk_text_view_set_editable (GTK_TEXT_VIEW (text_view), FALSE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (text_view), FALSE);

  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
  gtk_text_buffer_set_text (buffer, "Welcome to the chat!\n", -1);

  entry = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (box), entry, FALSE, FALSE, 0);
  g_object_set_data (G_OBJECT (entry), "text_view", text_view);

  send_button = gtk_button_new_with_label ("Send");
  gtk_box_pack_start (GTK_BOX (box), send_button, FALSE, FALSE, 0);
  g_signal_connect (send_button, "clicked", G_CALLBACK (send_button_clicked), entry);

    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_widget_show_all (window);

    gtk_main ();

    return 0;
}
