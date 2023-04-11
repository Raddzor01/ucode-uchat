#include "../inc/client.h"

GtkWidget *password_entry;
GtkWidget *username_entry;

bool chech_fields(const gchar *username, const gchar *password) {

    if (mx_strlen(username) < MIN_NUMBER_OF_CHARACTERS || mx_strlen(password) < MIN_NUMBER_OF_CHARACTERS) {
        pop_up_window("Fields must store at least one character");
        return false;
    }

    if (mx_strlen(username) > MAX_NUMBER_OF_CHARACTERS || mx_strlen(password) > MAX_NUMBER_OF_CHARACTERS) {
        pop_up_window("Password or login has more than 16 characters");
        return false;
    }
    
    return true;
}

void login_clicked(GtkWidget *widget) {

    if (info){}
        (void)widget;
    
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if (chech_fields(username, password) == false)
        return;

    if (send_login_to_server(username, password) == 1) {
        // Login successful
        chat_window();
    } else {
        // Login failed
        pop_up_window("Wrong password or login!");
    }
}

void signup_clicked(GtkWidget *widget) {

    if (info){}
        (void)widget;
    
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    if (chech_fields(username, password) == false)
        return;

    int output = send_sign_up_to_server(username, password);

    if (output == 0) {
        pop_up_window("Sign up successful");
        log_menu(main_window);
    } else if (output == 1) {
        pop_up_window("Error in server cJSON");
    } else if (output == 2) {
        pop_up_window("Invalid password");
    } else if (output == 3) {
        pop_up_window("This user already exists");
    } else if (output == -1) {
        pop_up_window("Error in client cJSON");
    }
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
    GtkWidget *chat_box;
    GtkWidget *input_box;
    GtkWidget *entry;
    GtkWidget *send_button;
    GtkWidget *scrolled_window;
    GtkWidget *file_selection;
    GtkWidget *box_container;
    GtkWidget *grid = get_widget_by_name_r(main_window, "chat_grid");

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(box, "chat");
    gtk_grid_attach(GTK_GRID(grid), box, 1, 0, 1, 1);

    chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request (chat_box, 500, 450);
    gtk_widget_set_name(chat_box, "chat_box");
    gtk_box_pack_start(GTK_BOX(box), chat_box, TRUE, TRUE, 0);

    gtk_widget_set_hexpand(chat_box, TRUE);
    gtk_widget_set_vexpand(chat_box, TRUE);
    gtk_widget_set_halign(chat_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(chat_box, GTK_ALIGN_FILL);

    input_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), input_box, TRUE, TRUE, 0);

    gtk_widget_set_size_request (input_box, -1, 20);
    gtk_widget_set_hexpand(input_box, TRUE);
    gtk_widget_set_halign(input_box, GTK_ALIGN_FILL);
    gtk_widget_set_vexpand(input_box, FALSE);
    gtk_widget_set_valign(input_box, GTK_ALIGN_START);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(chat_box), scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    add_class(scrolled_window, "scrl-win");
    gtk_widget_set_name(scrolled_window, "scroll");
    
    box_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(scrolled_window), box_container);
    gtk_widget_set_name(box_container, "box_holder");

    entry = gtk_entry_new();
    gtk_widget_set_size_request (entry, 400, 10);
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_widget_set_halign(entry, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(input_box), entry, TRUE, TRUE, 0);
    add_class(entry, "chat_text_entry");
    gtk_widget_set_name(entry, "chat_text_entry");

    gtk_box_pack_start(GTK_BOX(input_box), entry, FALSE, FALSE, 0);

    g_signal_connect(entry, "activate", G_CALLBACK(send_message), NULL);

    file_selection = gtk_button_new_with_label("...");
    gtk_box_pack_start(GTK_BOX(input_box), file_selection, FALSE, FALSE, 0);
    g_signal_connect(file_selection, "clicked", G_CALLBACK(file_select), NULL);

    send_button = gtk_button_new_with_label("Send");
    gtk_widget_set_size_request (send_button, 100, -1);
    gtk_box_pack_start(GTK_BOX(input_box), send_button, FALSE, FALSE, 0);
    gtk_widget_set_name(send_button, "send_button");
    info->entry = entry;

    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), NULL);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void build_users(GtkWidget *grid) {

    GtkWidget *users_box;
    GtkWidget *scrolled_window;
    GtkWidget *box_for_users;
    GtkWidget *search_box;

    users_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    add_class(users_box, "user-box");
    gtk_widget_set_hexpand(users_box, FALSE);
    gtk_widget_set_vexpand(users_box, TRUE);
    gtk_widget_set_valign(users_box, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(grid), users_box, 0, 0, 1, 1);

    search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(users_box), search_box, FALSE, FALSE, 0);

    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(search_box), entry, TRUE, TRUE, 0);
    g_signal_connect(entry, "changed", G_CALLBACK(find_chats), NULL);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request (scrolled_window, 200, 400);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_widget_set_valign(scrolled_window, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(users_box), scrolled_window, TRUE, TRUE, 0);

    box_for_users = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrolled_window), box_for_users);
    gtk_widget_set_name(box_for_users, "box_for_users");

    for (int i = account->chat_count - 1; i >= 0; i--) {
        user_box(i);
    }

    //chert moment
    GError *error = NULL;
    GdkPixbuf *pixbuf;

    GtkWidget *user_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(users_box), user_info_box, TRUE, TRUE, 0);
    gtk_widget_set_size_request (user_info_box, -1, 20);
    gtk_widget_set_hexpand(user_info_box, TRUE);
    gtk_widget_set_halign(user_info_box, GTK_ALIGN_FILL);
    gtk_widget_set_vexpand(user_info_box, FALSE);
    gtk_widget_set_valign(user_info_box, GTK_ALIGN_START);

    pixbuf = gdk_pixbuf_new_from_file("Client/Ass/HOG.png", &error);
    if (error != NULL)
        g_error("Error loading image: %s", error->message);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);

    GtkWidget *avatar = gtk_image_new_from_pixbuf(pixbuf);
    gtk_image_set_pixel_size(GTK_IMAGE(avatar), 64);
    gtk_widget_set_size_request(avatar, 64, 64);
    gtk_box_pack_start(GTK_BOX(user_info_box), avatar, FALSE, FALSE, 0);

    GtkWidget *name_label = gtk_label_new(account->username);
    gtk_widget_set_size_request (user_info_box, -1, 10);
    gtk_box_pack_start(GTK_BOX(user_info_box), name_label, FALSE, FALSE, 10);
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

    if (popup_window != NULL)
        return;

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
