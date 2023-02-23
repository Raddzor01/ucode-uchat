#include "../inc/client.h"

GtkWidget *password_entry;
GtkWidget *username_entry;

void login_clicked(GtkWidget *widget, t_info *info) {
    if(info){}
        (void)widget;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if (g_strcmp0(username, "user") == 0 && g_strcmp0(password, "pass") == 0) {
        // Login successful
        chat_window(info);
    } else {
        // Login failed
    }
}

void signup_clicked(GtkWidget *widget, t_info *info) {
    if(info){}
        (void)widget;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if (g_strcmp0(username, "user") == 0 && g_strcmp0(password, "pass") == 0) {
        // Login successful
        chat_window(info);
    } else {
        // Login failed
    }
}

void build_login(t_info *info) {
    GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add (GTK_CONTAINER (main_window), box);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(box), username_label, FALSE, FALSE, 0);

    username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), username_entry, FALSE, FALSE, 0);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(box), password_label, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(box), password_entry, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *signup_button = gtk_button_new_with_label("Sign up");
    gtk_box_pack_start(GTK_BOX(box), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), signup_button, FALSE, FALSE, 0);
    g_signal_connect(login_button, "clicked", G_CALLBACK(login_clicked), info);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(signup_clicked), info);

    g_signal_connect (main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
}

void build_signup(t_info *info) {
    GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add (GTK_CONTAINER (main_window), box);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(box), username_label, FALSE, FALSE, 0);

    username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), username_entry, FALSE, FALSE, 0);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(box), password_label, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(box), password_entry, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Back");
    GtkWidget *signup_button = gtk_button_new_with_label("Sign up");
    gtk_box_pack_start(GTK_BOX(box), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), signup_button, FALSE, FALSE, 0);
    g_signal_connect(login_button, "clicked", G_CALLBACK(log_menu), &info);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(signup_clicked), &info);

    g_signal_connect (main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
}

void build_chat_window(t_info *info) {

    GtkWidget *box;
    GtkWidget *entry;
    GtkWidget *text_view;
    GtkWidget *send_button;
    GtkWidget *scrolled_window;

    create_new_window("UChat", 500, 400, true);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(main_window), box);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();

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
}


