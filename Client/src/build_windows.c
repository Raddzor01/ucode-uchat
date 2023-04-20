#include "../inc/client.h"

GtkWidget *password_entry;
GtkWidget *username_entry;

bool check_fields_size(const gchar *username, const gchar *password, GtkWidget *username_error_label, GtkWidget *password_error_label)
{
    bool error = false;
    if (!mx_strcmp(username, ""))
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "Field must store at least one character");
        error = true;
    }

    if (!mx_strcmp(password, ""))
    {
        gtk_label_set_text(GTK_LABEL(password_error_label), "Field must store at least one character");
        error = true;
    }

    if(error)
        return true;

    if (mx_strlen(username) > MAX_NUMBER_OF_CHARACTERS)
    {
        gtk_label_set_text(GTK_LABEL(password_error_label), "Login has more than 16 characters");
        error = true;
    }

    if (mx_strlen(password) > MAX_NUMBER_OF_CHARACTERS)
    {
        gtk_label_set_text(GTK_LABEL(password_error_label), "Password has more than 16 characters");
        error = true;
    }
    return error;
}

void login_clicked(GtkWidget *__attribute__((unused)) widget)
{
    GtkWidget *box = get_widget_by_name_r(main_window, "box");
    GtkWidget *username_error_label = get_widget_by_name_r(box, "username_error_label");
    GtkWidget *password_error_label = get_widget_by_name_r(box, "password_error_label");

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    gtk_label_set_text(GTK_LABEL(username_error_label), " ");
    gtk_label_set_text(GTK_LABEL(password_error_label), " ");

    if (check_fields_size(username, password,  username_error_label, password_error_label) == true)
        return;

    if (send_login_to_server(username, password) == 1)
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "Login successful");
        chat_window();
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "Wrong password or login!");
    }
}

void signup_clicked(GtkWidget *__attribute__((unused)) widget)
{
    GtkWidget *box = get_widget_by_name_r(main_window, "box");
    GtkWidget *username_error_label = get_widget_by_name_r(box, "username_error_label");
    GtkWidget *password_error_label = get_widget_by_name_r(box, "password_error_label");

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    gtk_label_set_text(GTK_LABEL(username_error_label), " ");
    gtk_label_set_text(GTK_LABEL(password_error_label), " ");

    if (check_fields_size(username, password,  username_error_label, password_error_label) == true)
        return;

    int error_code = send_sign_up_to_server(username, password);
    if (error_code == ERR_SUCCESS)
    {
        pop_up_window("Sign up successful");
        log_menu(main_window);
    } else if (error_code == ERR_USER_EXISTS)
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "This user already exists");
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(username_error_label),"Error while sending sign up request");
    }
}

void build_login()
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(box, "box");
    gtk_container_add(GTK_CONTAINER(main_window), box);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(box), username_label, FALSE, FALSE, 0);

    username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), username_entry, FALSE, FALSE, 0);

    GtkWidget *username_error_label = gtk_label_new(" ");
    gtk_widget_set_name(username_error_label, "username_error_label");
    add_class(username_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(box), username_error_label, FALSE, FALSE, 0);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(box), password_label, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(box), password_entry, FALSE, FALSE, 0);

    GtkWidget *password_error_label = gtk_label_new(" ");
    gtk_widget_set_name(password_error_label, "password_error_label");
    add_class(password_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(box), password_error_label, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *signup_button = gtk_button_new_with_label("Sign up");
    gtk_box_pack_start(GTK_BOX(box), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), signup_button, FALSE, FALSE, 0);
    g_signal_connect(login_button, "clicked", G_CALLBACK(login_clicked), NULL);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(sign_up_menu), NULL);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void build_signup()
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), box);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(box), username_label, FALSE, FALSE, 0);

    username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), username_entry, FALSE, FALSE, 0);

    GtkWidget *username_error_label = gtk_label_new(" ");
    gtk_widget_set_name(username_error_label, "username_error_label");
    add_class(username_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(box), username_error_label, FALSE, FALSE, 0);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(box), password_label, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(box), password_entry, FALSE, FALSE, 0);

    GtkWidget *password_error_label = gtk_label_new(" ");
    gtk_widget_set_name(password_error_label, "password_error_label");
    add_class(password_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(box), password_error_label, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Back");
    GtkWidget *signup_button = gtk_button_new_with_label("Sign up");
    gtk_box_pack_start(GTK_BOX(box), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), signup_button, FALSE, FALSE, 0);
    g_signal_connect(login_button, "clicked", G_CALLBACK(log_menu), NULL);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(signup_clicked), NULL);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void chat_info()
{
    GtkWidget *box = get_widget_by_name_r(main_window, "chat_info");
    clear_box(box);
    GtkWidget *image;
    GtkWidget *chat_name;
    GtkWidget *chat_info;
    GtkWidget *text_box;

    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file("Client/data/default_image.png", NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 40, 40, GDK_INTERP_BILINEAR);

    image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);

    chat_name = gtk_label_new(account->current_chat->name);

    chat_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(chat_info, -1, 40);
    gtk_widget_set_halign(chat_info, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(box), chat_info, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(chat_info), image, FALSE, FALSE, 0);

    text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(text_box, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(chat_info), text_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(text_box), chat_name, FALSE, FALSE, 0);

    gtk_widget_show_all(box);
}

void build_chat_window()
{
    if (get_widget_by_name_r(main_window, "chat"))
        return;

    GtkWidget *box;
    GtkWidget *chat_box;
    GtkWidget *input_box;
    GtkWidget *entry;
    GtkWidget *send_button;
    GtkWidget *scrolled_window;
    // GtkWidget *file_selection;
    GtkWidget *box_container;
    GtkWidget *grid = get_widget_by_name_r(main_window, "chat_grid");
    GtkWidget *chat_info;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(box, "chat");
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(box, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(grid), box, 1, 0, 1, 1);

    chat_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(chat_info, "chat_info");
    gtk_box_pack_start(GTK_BOX(box), chat_info, FALSE, FALSE, 0);

    chat_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(chat_box, 500, 450);
    gtk_widget_set_name(chat_box, "chat_box");
    gtk_box_pack_start(GTK_BOX(box), chat_box, TRUE, TRUE, 0);

    input_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), input_box, FALSE, FALSE, 0);
    gtk_widget_set_size_request(input_box, -1, 20);
    gtk_widget_set_vexpand(input_box, FALSE);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(chat_box), scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    add_class(scrolled_window, "scrl-win");
    gtk_widget_set_name(scrolled_window, "scroll");

    box_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(scrolled_window), box_container);
    gtk_widget_set_name(box_container, "box_holder");

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Message");
    gtk_box_pack_start(GTK_BOX(input_box), entry, TRUE, TRUE, 0);
    gtk_widget_set_size_request(entry, 400, 10);
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_widget_set_halign(entry, GTK_ALIGN_FILL);
    add_class(entry, "chat_text_entry");
    gtk_widget_set_name(entry, "chat_text_entry");

    g_signal_connect(entry, "activate", G_CALLBACK(send_message), NULL);

    // file_selection = gtk_button_new_with_label("...");
    // gtk_box_pack_start(GTK_BOX(input_box), file_selection, FALSE, FALSE, 0);
    // g_signal_connect(file_selection, "clicked", G_CALLBACK(file_select), NULL);

    send_button = gtk_button_new_with_label("Send");
    gtk_widget_set_size_request(send_button, 100, -1);
    gtk_box_pack_start(GTK_BOX(input_box), send_button, FALSE, FALSE, 0);
    gtk_widget_set_name(send_button, "send_button");
    info->entry = entry;

    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), NULL);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(box);
}

void logout_event(GtkWidget *__attribute__((unused)) widget)
{
    send_logout_to_server();
}

char *get_user_image(int image_id)
{
    if (image_id == 1)
        return DEFAULT_IMAGE;
    if (access(account->image_path, F_OK) == 0)
        return account->image_path;

    if (!get_image_from_server(image_id))
        return account->image_path;
    return DEFAULT_IMAGE;
}

void build_users(GtkWidget *grid)
{
    GtkWidget *users_box;
    GtkWidget *scrolled_window;
    GtkWidget *box_for_users;
    GtkWidget *search_box;
    GtkWidget *create_chat;

    users_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    add_class(users_box, "user-box");
    gtk_widget_set_hexpand(users_box, FALSE);
    gtk_widget_set_vexpand(users_box, TRUE);
    gtk_widget_set_valign(users_box, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(grid), users_box, 0, 0, 1, 1);

    search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(users_box), search_box, FALSE, FALSE, 0);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Search");
    gtk_box_pack_start(GTK_BOX(search_box), entry, TRUE, TRUE, 0);
    g_signal_connect(entry, "changed", G_CALLBACK(find_chats), NULL);

    create_chat = create_image_button("Client/icons/plus.png", 16, 16);
    add_class(create_chat, "plus_button");
    gtk_widget_set_size_request(create_chat, 16, 16);
    gtk_box_pack_start(GTK_BOX(search_box), create_chat, FALSE, FALSE, 0);
    g_signal_connect(create_chat, "clicked", G_CALLBACK(create_chat_menu), NULL);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, 200, 400);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_widget_set_valign(scrolled_window, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(users_box), scrolled_window, TRUE, TRUE, 0);

    box_for_users = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrolled_window), box_for_users);
    gtk_widget_set_name(box_for_users, "box_for_users");

    display_users();

    // chert moment
    GError *error = NULL;
    GdkPixbuf *pixbuf;

    GtkWidget *user_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(users_box), user_info_box, FALSE, FALSE, 0);
    gtk_widget_set_size_request(user_info_box, -1, 20);

    pixbuf = gdk_pixbuf_new_from_file(get_user_image(account->image_id), &error);
    if (error != NULL)
        g_error("Error loading image: %s", error->message);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);

    GtkWidget *avatar = gtk_image_new_from_pixbuf(pixbuf);
    gtk_image_set_pixel_size(GTK_IMAGE(avatar), 64);
    gtk_widget_set_size_request(avatar, 64, 64);
    gtk_box_pack_start(GTK_BOX(user_info_box), avatar, FALSE, FALSE, 0);

    GtkWidget *text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(user_info_box), text_box, FALSE, FALSE, 10);

    GtkWidget *name_label = gtk_label_new(account->username);
    gtk_widget_set_size_request(user_info_box, -1, 10);
    gtk_widget_set_valign(name_label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(text_box), name_label, FALSE, FALSE, 10);

    GtkWidget *link_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(text_box), link_box, FALSE, FALSE, 10);
    add_class(link_box, "links");

    GtkWidget *edit_profile_box = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(link_box), edit_profile_box, FALSE, FALSE, 10);

    GtkWidget *edit_profile = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(edit_profile), "<u>Edit Profile</u>");
    gtk_container_add(GTK_CONTAINER(edit_profile_box), edit_profile);

    g_signal_connect(edit_profile_box, "button_press_event", G_CALLBACK(build_edit_profile), NULL);

    GtkWidget *log_out_box = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(link_box), log_out_box, FALSE, FALSE, 10);

    GtkWidget *log_out = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(log_out), "<u>Log Out</u>");
    gtk_container_add(GTK_CONTAINER(log_out_box), log_out);

    g_signal_connect(log_out_box, "button_press_event", G_CALLBACK(logout_event), NULL);
    g_signal_connect(log_out_box, "button_press_event", G_CALLBACK(log_menu), NULL);
}

void pop_up_window(char *text)
{
    GtkWidget *popup = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(popup), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(popup), 200, 40);
    gtk_window_set_resizable(GTK_WINDOW(popup), FALSE);

    GtkWidget *label = gtk_label_new(text);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(vbox), label);
    gtk_widget_set_hexpand(vbox, TRUE);
    gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(vbox, FALSE);
    gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);

    gtk_container_add(GTK_CONTAINER(popup), vbox);

    gtk_widget_show_all(popup);
}

static GtkWidget *popup_window = NULL;

static gboolean close_popup_window(gpointer data)
{

    if (data)
    {
    }

    gtk_widget_destroy(popup_window);
    popup_window = NULL;

    return G_SOURCE_REMOVE;
}

void hog()
{
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
    GtkWidget *image = gtk_image_new_from_file("Client/data/default_image.png");
    gtk_container_add(GTK_CONTAINER(popup_window), image);

    // Show the window
    gtk_widget_show_all(popup_window);

    // Set a timer to close the window after 1 second
    g_timeout_add(100, close_popup_window, NULL);
}

bool window_check = false;

void close_window_by_button(GtkButton *button, gpointer *data)
{
    GtkWidget *window = GTK_WIDGET(data);
    gtk_widget_destroy(window);

    window_check = false;
    (void)button;
}

void close_window(gpointer *data)
{
    GtkWidget *window = GTK_WIDGET(data);
    gtk_widget_destroy(window);
    window_check = false;
}

void create_chat_menu()
{
    if (window_check == true)
        return;

    GtkWidget *window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 1);

    gtk_widget_set_name(window, "create_chat");

    g_signal_connect(window, "delete_event", G_CALLBACK(close_window), window);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *chatname_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(box), chatname_label, FALSE, FALSE, 0);

    GtkWidget *chatname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(chatname_entry), "Chat name");
    gtk_box_pack_start(GTK_BOX(box), chatname_entry, FALSE, FALSE, 0);

    GtkWidget *make_chat_button = gtk_button_new_with_label("create chat");
    gtk_box_pack_start(GTK_BOX(box), make_chat_button, FALSE, FALSE, 0);
    g_signal_connect(make_chat_button, "clicked", G_CALLBACK(create_chat), chatname_entry);
    g_signal_connect(make_chat_button, "clicked", G_CALLBACK(close_window_by_button), window);

    gtk_widget_show_all(window);
    window_check = true;
}

void display_users()
{
    t_chat *chat = account->chats;
    while (chat != NULL)
    {
        user_box(chat, false);
        chat = chat->next;
    }
}

GtkWidget *edit_window;

void build_edit_profile()
{
    if (window_check == true)
        return;

    // GtkWidget *window;

    edit_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(edit_window), 300, 1);
    g_signal_connect(edit_window, "delete_event", G_CALLBACK(close_window), edit_window);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(edit_window), box);

    GtkWidget *surprize = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(surprize), "<a href=\"https://matias.ma/nsfw/\">Click me!</a>");
    gtk_label_set_use_markup(GTK_LABEL(surprize), TRUE);
    add_class(surprize, "links");
    gtk_widget_set_halign(surprize, GTK_ALIGN_END);
    gtk_widget_set_valign(surprize, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), surprize, FALSE, FALSE, 0);

    GtkWidget *profile_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), profile_info, FALSE, FALSE, 0);

    GtkWidget *edit_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), edit_box, FALSE, FALSE, 0);
    gtk_widget_set_name(edit_box, "edit");

    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file("Client/data/default_image.png", NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 60, 60, GDK_INTERP_BILINEAR);

    GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);
    gtk_box_pack_start(GTK_BOX(profile_info), image, FALSE, FALSE, 0);

    GtkWidget *text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(profile_info), text_box, FALSE, FALSE, 0);

    GtkWidget *name = gtk_text_view_new();
    add_class(text_box, "edit");
    gtk_widget_set_name(name, "username");
    gtk_widget_set_halign(name, GTK_ALIGN_START);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(name), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(name), FALSE);
    gtk_box_pack_start(GTK_BOX(text_box), name, FALSE, FALSE, 0);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(name));
    gtk_text_buffer_set_text(buffer, account->username, -1);

    GtkWidget *edit_profile_box = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(text_box), edit_profile_box, FALSE, FALSE, 10);
    GtkWidget *edit_profile = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(edit_profile), "<u>Change Name or Password</u>");
    gtk_container_add(GTK_CONTAINER(edit_profile_box), edit_profile);
    add_class(edit_profile, "links");

    g_signal_connect(edit_profile_box, "button_press_event", G_CALLBACK(edit_username), NULL);

    gtk_widget_show_all(edit_window);
    window_check = true;
}

void edit_username()
{
    GtkWidget *box = get_widget_by_name_r(edit_window, "edit");
    clear_box(box);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_widget_set_valign(username_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), username_label, FALSE, FALSE, 0);

    GtkWidget *name_entry = gtk_entry_new();
    gtk_widget_set_name(name_entry, "username_entry");
    gtk_entry_set_text(GTK_ENTRY(name_entry), account->username);
    gtk_box_pack_start(GTK_BOX(box), name_entry, FALSE, FALSE, 0);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_widget_set_valign(password_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), password_label, FALSE, FALSE, 0);

    GtkWidget *pass_entry = gtk_entry_new();
    gtk_widget_set_name(pass_entry, "password_entry");
    gtk_entry_set_visibility(GTK_ENTRY(pass_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(box), pass_entry, FALSE, FALSE, 0);

    GtkWidget *accept_button = gtk_button_new_with_label("Accept");
    gtk_box_pack_start(GTK_BOX(box), accept_button, FALSE, FALSE, 0);
    g_signal_connect(accept_button, "clicked", G_CALLBACK(accept_clicked), edit_window);
    gtk_widget_show_all(box);
}
