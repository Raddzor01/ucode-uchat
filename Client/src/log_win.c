#include "../inc/client.h"

GtkWidget *username_label;
GtkWidget *username_entry;
GtkWidget *password_label;
GtkWidget *password_entry;
GtkWidget *login_button;

void login_clicked(GtkWidget *widget, t_info *info) {
    if(info){}
        (void)widget;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if (g_strcmp0(username, "user") == 0 && g_strcmp0(password, "pass") == 0) {
        // Login successful
        chat_window(*info);
    } else {
        // Login failed
    }
}

GtkWidget* create_new_window(char *title, int width, int height, bool resizable)
{
	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), title);
	gtk_window_set_default_size(GTK_WINDOW(window), width, height);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window), resizable);
    g_signal_connect(window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);

	return window;
}

void log_menu(t_info info) {

    if (main_window)
        gtk_widget_destroy(main_window);

    // main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    // gtk_window_set_title (GTK_WINDOW (main_window), "Login");
    // gtk_window_set_default_size (GTK_WINDOW (main_window), 400, 200);
    main_window = create_new_window("Login", 500, 0, false);

    GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add (GTK_CONTAINER (main_window), box);

    username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(box), username_label, FALSE, FALSE, 0);

    username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), username_entry, FALSE, FALSE, 0);

    password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(box), password_label, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(box), password_entry, FALSE, FALSE, 0);

    login_button = gtk_button_new_with_label("Login");
    gtk_box_pack_start(GTK_BOX(box), login_button, FALSE, FALSE, 0);
    g_signal_connect(login_button, "clicked", G_CALLBACK(login_clicked), &info);

    g_signal_connect (main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    gtk_widget_show_all (main_window);
}
