#include "../inc/client.h"

GtkWidget *password_entry;
GtkWidget *username_entry;

void login_clicked(GtkWidget *widget) {
    if(info){}
        (void)widget;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if ((mx_strlen(username) || mx_strlen(password)) <= 0) {
        pop_up_window("Fields must store at least one character");
        return;
    }

    if (send_login_to_server(username, password) == 1) {
        // Login successful
        chat_window(info);
    } else {
        // Login failed
        pop_up_window("Wrong password or login!");
    }
}

void signup_clicked(GtkWidget *widget) {
    if(info){}
        (void)widget;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if ((mx_strlen(username) || mx_strlen(password)) <= 0) {
        pop_up_window("Fields must store at least one character");
        return;
    }

    // if (send_login_to_server(username, password) == 1) {
    //     // Login successful
    //     // chat_window(info);
    //     pop_up_window("This user already exists!");
    // } else {
    //     // Login failed
        send_sign_up_to_server(username, password);
    // }
}

void build_login() {
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
    add_class(login_button, "button");
    GtkWidget *signup_button = gtk_button_new_with_label("Sign up");
    add_class(signup_button, "button");
    gtk_box_pack_start(GTK_BOX(box), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), signup_button, FALSE, FALSE, 0);
    g_signal_connect(login_button, "clicked", G_CALLBACK(login_clicked), NULL);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(sign_up_menu), NULL);

    g_signal_connect (main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
}

void build_signup() {
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
    g_signal_connect(login_button, "clicked", G_CALLBACK(log_menu), NULL);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(signup_clicked), NULL);

    g_signal_connect (main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
}

void build_chat_window() {

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
    add_class(scrolled_window, "scrl-win");

    text_view = gtk_text_view_new();
    add_class(text_view, "text_view");

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
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_button_clicked), NULL);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void pop_up_window(char *text) {
    GtkWidget *popup = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(popup), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(popup), 200, 40);
    gtk_window_set_resizable(GTK_WINDOW(popup), FALSE);

    GtkWidget *label = gtk_label_new(text);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(vbox), label);

    gtk_container_add(GTK_CONTAINER(popup), vbox);

    gtk_widget_show_all(popup);
}

static GtkWidget *popup_window = NULL;

static gboolean close_popup_window(gpointer data) {
    if(data){}
    gtk_widget_destroy(popup_window);
    popup_window = NULL;
    return G_SOURCE_REMOVE;
}

void hog() {
    if (popup_window != NULL) {
        return;
    }

    // Create a new window
    popup_window = gtk_window_new(GTK_WINDOW_POPUP);
    add_class(popup_window, "pop");
    gtk_window_set_title(GTK_WINDOW(popup_window), "Popup Window");
    gtk_window_set_position(GTK_WINDOW(popup_window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_resizable(GTK_WINDOW(popup_window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(popup_window), 10);

    // Create an image widget and add it to the window
    GtkWidget *image = gtk_image_new_from_file("Client/Ass/HOG.png");
    gtk_container_add(GTK_CONTAINER(popup_window), image);

    // Show the window
    gtk_widget_show_all(popup_window);

    // Set a timer to close the window after 1 second
    g_timeout_add(100, close_popup_window, NULL);
}

