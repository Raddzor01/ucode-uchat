#include "../inc/client.h"

GtkWidget *password_entry;
GtkWidget *username_entry;
GtkWidget *edit_window;

char *get_user_image(int image_id, int pfp_type)
{
    if (image_id == 1)
    {
        switch (pfp_type)
        {
        case CHAT_NORMAL:
            return DEFAULT_CHAT_IMAGE;
            break;
        case CHAT_SAVED:
            return SAVED_IMAGE;
            break;
        case PFP_USER:
            return DEFAULT_IMAGE;
            break;
        }
        return DEFAULT_IMAGE;
    }
    sem_wait(&account->semaphore);
    char *path = get_image_from_server(image_id);
    sem_post(&account->semaphore);
    return path;
}

bool check_field_size(const gchar *field, GtkWidget *field_widget)
{
    bool error = false;
    if (!mx_strcmp(field, ""))
    {
        gtk_label_set_text(GTK_LABEL(field_widget), "Field must store at least one character");
        error = true;
    }

    if (mx_strlen(field) > MAX_NUMBER_OF_CHARACTERS)
    {
        gtk_label_set_text(GTK_LABEL(field), "Password has more than 32 characters");
        error = true;
    }

    if (check_str_for_spec_char(field) == false)
    {
        gtk_label_set_text(GTK_LABEL(field_widget), "Special characters not allowed. Please try again.");
        error = true;
    }

    return error;
}

void login_clicked(GtkWidget *__attribute__((unused)) widget)
{
    bool error = false;

    GtkWidget *box = get_widget_by_name_r(main_window, "login_window");
    GtkWidget *username_error_label = get_widget_by_name_r(box, "username_error_label");
    GtkWidget *password_error_label = get_widget_by_name_r(box, "password_error_label");

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    gtk_label_set_text(GTK_LABEL(username_error_label), " ");
    gtk_label_set_text(GTK_LABEL(password_error_label), " ");

    if (check_field_size(username, username_error_label))
        error = true;

    if (check_field_size(password, password_error_label))
        error = true;

    if (error)
        return;

    unsigned char hash_password[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password, mx_strlen(password), hash_password);
    int error_code = send_login_to_server(username, hash_password);
    if (error_code == ERR_SUCCESS)
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "Login successful");
        chat_window();
    }
    else if (error_code == ERR_JSON)
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "Error while trying to login. Please try again.");
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "Wrong password or login!");
    }
}

void signup_clicked(GtkWidget *__attribute__((unused)) widget)
{
    bool error = false;

    GtkWidget *box = get_widget_by_name_r(main_window, "signup_window");
    GtkWidget *username_error_label = get_widget_by_name_r(box, "signup_username_error_label");
    GtkWidget *password_error_label = get_widget_by_name_r(box, "signup_password_error_label");
    GtkWidget *confirm_password_error_label = get_widget_by_name_r(box, "confirm_password_error_label");

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
    const gchar *confirm_password = gtk_entry_get_text(GTK_ENTRY(get_widget_by_name_r(box, "confirm_password_entery")));

    gtk_label_set_text(GTK_LABEL(username_error_label), " ");
    gtk_label_set_text(GTK_LABEL(password_error_label), " ");
    gtk_label_set_text(GTK_LABEL(confirm_password_error_label), " ");

    if (check_field_size(username, username_error_label))
        error = true;

    if (check_field_size(password, password_error_label))
        error = true;

    if (check_field_size(confirm_password, confirm_password_error_label))
        error = true;

    if (error)
        return;

    if (mx_strcmp(password, confirm_password))
    {
        gtk_label_set_text(GTK_LABEL(confirm_password_error_label), "The passwords you entered do not match. Please try again.");
        return;
    }

    unsigned char hash_password[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password, mx_strlen(password), hash_password);
    int error_code = send_sign_up_to_server(username, hash_password);
    if (error_code == ERR_SUCCESS)
    {
        pop_up_window("Sign up successful");
        log_menu(main_window);
    }
    else if (error_code == ERR_USER_EXISTS)
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "This user already exists");
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(username_error_label), "Error while sending sign up request");
    }
}

void build_login()
{
    GtkWidget *login_window = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(login_window, "login_window");
    gtk_container_add(GTK_CONTAINER(main_window), login_window);

    GtkWidget *above = gtk_label_new(" ");
    gtk_box_pack_start(GTK_BOX(login_window), above, FALSE, FALSE, 0);

    GtkWidget *greeting_label = gtk_label_new("Welcome to Chat!");
    gtk_box_pack_start(GTK_BOX(login_window), greeting_label, FALSE, FALSE, 0);

    GtkWidget *under = gtk_label_new(" ");
    gtk_box_pack_start(GTK_BOX(login_window), under, FALSE, FALSE, 0);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(login_window), username_label, FALSE, FALSE, 0);

    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter username");
    gtk_box_pack_start(GTK_BOX(login_window), username_entry, FALSE, FALSE, 0);

    GtkWidget *username_error_label = gtk_label_new(" ");
    gtk_widget_set_name(username_error_label, "username_error_label");
    add_class(username_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(login_window), username_error_label, FALSE, FALSE, 0);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(login_window), password_label, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(login_window), password_entry, FALSE, FALSE, 0);

    GtkWidget *password_error_label = gtk_label_new(" ");
    gtk_widget_set_name(password_error_label, "password_error_label");
    add_class(password_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(login_window), password_error_label, FALSE, FALSE, 0);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *signup_button = gtk_button_new_with_label("Sign up");
    gtk_box_pack_start(GTK_BOX(login_window), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(login_window), signup_button, FALSE, FALSE, 0);

    g_signal_connect(login_button, "clicked", G_CALLBACK(login_clicked), NULL);
    g_signal_connect(signup_button, "clicked", G_CALLBACK(sign_up_menu), NULL);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void build_signup()
{
    GtkWidget *signup_window = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(signup_window, "signup_window");
    gtk_container_add(GTK_CONTAINER(main_window), signup_window);

    GtkWidget *above = gtk_label_new(" ");
    gtk_box_pack_start(GTK_BOX(signup_window), above, FALSE, FALSE, 0);

    GtkWidget *greeting_label = gtk_label_new("Register to Chat");
    gtk_box_pack_start(GTK_BOX(signup_window), greeting_label, FALSE, FALSE, 0);

    GtkWidget *under = gtk_label_new(" ");
    gtk_box_pack_start(GTK_BOX(signup_window), under, FALSE, FALSE, 0);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(signup_window), username_label, FALSE, FALSE, 0);

    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter username");
    gtk_box_pack_start(GTK_BOX(signup_window), username_entry, FALSE, FALSE, 0);

    GtkWidget *username_error_label = gtk_label_new(" ");
    gtk_widget_set_name(username_error_label, "signup_username_error_label");
    add_class(username_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(signup_window), username_error_label, FALSE, FALSE, 0);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(signup_window), password_label, FALSE, FALSE, 0);

    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(signup_window), password_entry, FALSE, FALSE, 0);

    GtkWidget *password_error_label = gtk_label_new(" ");
    gtk_widget_set_name(password_error_label, "signup_password_error_label");
    add_class(password_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(signup_window), password_error_label, FALSE, FALSE, 0);

    GtkWidget *confirm_password_label = gtk_label_new("Repeat password:");
    gtk_box_pack_start(GTK_BOX(signup_window), confirm_password_label, FALSE, FALSE, 0);

    GtkWidget *confirm_password_entry = gtk_entry_new();
    gtk_widget_set_name(confirm_password_entry, "confirm_password_entery");
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password_entry), "Enter password one more time...");
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE);
    gtk_box_pack_start(GTK_BOX(signup_window), confirm_password_entry, FALSE, FALSE, 0);

    GtkWidget *confirm_password_error_label = gtk_label_new(" ");
    gtk_widget_set_name(confirm_password_error_label, "confirm_password_error_label");
    add_class(confirm_password_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(signup_window), confirm_password_error_label, FALSE, FALSE, 0);

    GtkWidget *signup_button = gtk_button_new_with_label("Sign up");
    GtkWidget *login_button = gtk_button_new_with_label("Back");
    gtk_box_pack_start(GTK_BOX(signup_window), signup_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(signup_window), login_button, FALSE, FALSE, 0);

    g_signal_connect(signup_button, "clicked", G_CALLBACK(signup_clicked), NULL);
    g_signal_connect(login_button, "clicked", G_CALLBACK(log_menu), NULL);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void delete_chat_confirm()
{
    if (delete_chat_in_server() != 0)
        return;

    pthread_mutex_lock(&account->mutex);
    chat_pop_by_id(&account->chats, account->current_chat->id);
    pthread_cancel(account->server_update_thread);
    pthread_create(&account->server_update_thread, NULL, server_update_thread, NULL);
    pthread_mutex_unlock(&account->mutex);

    GtkWidget *chat = get_widget_by_name_r(main_window, "chat");
    GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");

    gtk_widget_destroy(chat);
    clear_box(box);
    empty_left_bar();
    empty_right_bar();
    display_users();

    gtk_widget_show_all(main_window);
}

void confirm_window(GtkWidget *__attribute__((unused)) button)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                    "Are you sure?");
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_YES)
    {
        delete_chat_confirm();
        gtk_widget_destroy(edit_window);
        window_check = false;
    }
    else if (result == GTK_RESPONSE_NO)
        pop_up_window("Something went wrong\nTry again");
    gtk_widget_destroy(dialog);
}

void chat_info()
{
    GtkWidget *box = get_widget_by_name_r(main_window, "chat_info");
    clear_box(box);
    GtkWidget *image;
    GtkWidget *chat_name;

    GdkPixbuf *pixbuf;

    pixbuf = gdk_pixbuf_new_from_file(access(account->current_chat->image_path, F_OK) == 0 ? account->current_chat->image_path : get_user_image(account->current_chat->image_id, account->current_chat->chat_type), NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 40, 40, GDK_INTERP_BILINEAR);

    image = gtk_image_new_from_pixbuf(pixbuf);
    add_class(image, "image");

    gtk_widget_set_name(image, "chat_pfp_image");

    GtkWidget *text_box = gtk_box_new(account->current_chat->chat_type == CHAT_SAVED ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL, 0);

    chat_name = gtk_label_new(account->current_chat->name);

    gtk_widget_set_halign(image, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), text_box, FALSE, FALSE, 0);

    // g_signal_connect(image, "clicked", G_CALLBACK(chat_menu), NULL);

    gtk_widget_set_halign(chat_name, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(text_box), chat_name, FALSE, FALSE, 0);

    if (account->current_chat->chat_type != CHAT_SAVED)
    {
        char *users_count_text = mx_strjoin("users: ", mx_itoa(account->current_chat->users_count));

        GtkWidget *user_count = gtk_label_new(users_count_text);
        mx_strdel(&users_count_text);
        add_class(user_count, "count");
        gtk_widget_set_halign(user_count, GTK_ALIGN_START);
        gtk_widget_set_valign(user_count, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(text_box), user_count, FALSE, FALSE, 0);
    }

    GtkWidget *settings_button = create_image_button("Client/icons/settings.png", 20, 20);
    add_class(settings_button, "image");
    gtk_widget_set_hexpand(settings_button, TRUE);
    gtk_widget_set_halign(settings_button, GTK_ALIGN_END);
    gtk_widget_set_valign(settings_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), settings_button, TRUE, TRUE, 0);

    g_signal_connect(settings_button, "clicked", G_CALLBACK(chat_menu), NULL);

    gtk_widget_show_all(box);
}

void build_chat_window()
{
    if (get_widget_by_name_r(main_window, "chat"))
        return;

    if (get_widget_by_name_r(main_window, "empty_right_bar"))
        gtk_widget_destroy(get_widget_by_name_r(main_window, "empty_right_bar"));

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
    gtk_widget_set_size_request(chat_info, 500, -1);
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
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Write message...");
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

    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
    gtk_adjustment_set_value(adjustment, gtk_adjustment_get_upper(adjustment) - gtk_adjustment_get_page_size(adjustment));

    gtk_widget_show_all(box);
}

void logout_event(GtkWidget *__attribute__((unused)) widget)
{
    send_logout_to_server();
    gtk_widget_destroy(edit_window);
    window_check = FALSE;
}

void build_users()
{
    GtkWidget *users_box;
    GtkWidget *scrolled_window;
    GtkWidget *box_for_users;
    GtkWidget *search_box;
    GtkWidget *create_chat;
    GtkWidget *grid = get_widget_by_name_r(main_window, "chat_grid");

    users_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    add_class(users_box, "user-box");
    gtk_widget_set_hexpand(users_box, FALSE);
    gtk_widget_set_vexpand(users_box, TRUE);
    gtk_widget_set_valign(users_box, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(grid), users_box, 0, 0, 1, 1);

    search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(users_box), search_box, FALSE, FALSE, 0);

    GtkWidget *entry = gtk_entry_new();
    gtk_widget_set_name(entry, "Search_entry");
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
    empty_left_bar();

    // chert moment
    GError *error = NULL;
    GdkPixbuf *pixbuf;

    GtkWidget *user_info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(users_box), user_info_box, FALSE, FALSE, 0);
    gtk_widget_set_size_request(user_info_box, -1, 20);
    add_class(user_info_box, "profile");

    pixbuf = gdk_pixbuf_new_from_file(access(account->image_path, F_OK) == 0 ? account->image_path : get_user_image(account->image_id, PFP_USER), &error);
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

    gtk_widget_show_all(users_box);
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
    GtkWidget *image = gtk_image_new_from_file("Client/icons/HOG.png");
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
        gtk_widget_destroy(edit_window);

    // GtkWidget *window;

    edit_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(edit_window), 300, 1);

    gtk_widget_set_name(edit_window, "create_chat");

    g_signal_connect(edit_window, "delete_event", G_CALLBACK(close_window), edit_window);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_name(box, "chatname_box");
    gtk_container_add(GTK_CONTAINER(edit_window), box);

    GtkWidget *chatname_label = gtk_label_new("Chatname:");

    gtk_box_pack_start(GTK_BOX(box), chatname_label, FALSE, FALSE, 0);

    GtkWidget *chatname_entry = gtk_entry_new();
    gtk_widget_set_name(chatname_entry, "chatname_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(chatname_entry), "Chat name");
    gtk_box_pack_start(GTK_BOX(box), chatname_entry, FALSE, FALSE, 0);

    GtkWidget *chatname_error_label = gtk_label_new(" ");
    gtk_widget_set_name(chatname_error_label, "chatname_error_label");
    add_class(chatname_error_label, "error-label");
    gtk_box_pack_start(GTK_BOX(box), chatname_error_label, FALSE, FALSE, 0);

    GtkWidget *make_chat_button = gtk_button_new_with_label("create chat");
    gtk_box_pack_start(GTK_BOX(box), make_chat_button, FALSE, FALSE, 0);
    g_signal_connect(make_chat_button, "clicked", G_CALLBACK(create_chat), edit_window);
    // g_signal_connect(make_chat_button, "clicked", G_CALLBACK(close_window_by_button), window);

    if (!chat_get_chat_by_type(account->chats, CHAT_SAVED))
    {
        GtkWidget *make_saved_button = gtk_button_new_with_label("created saved chat");
        gtk_box_pack_start(GTK_BOX(box), make_saved_button, FALSE, FALSE, 0);
        g_signal_connect(make_saved_button, "clicked", G_CALLBACK(create_saved), NULL);
        g_signal_connect(make_saved_button, "clicked", G_CALLBACK(close_window_by_button), edit_window);
    }
    gtk_widget_show_all(edit_window);
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

void build_edit_profile()
{
    if (window_check == true)
        gtk_widget_destroy(edit_window);

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

    GtkWidget *image = create_image_button(access(account->image_path, F_OK) == 0 ? account->image_path : get_user_image(account->image_id, PFP_USER), 60, 60);
    gtk_widget_set_name(image, "chat_menu_image");
    gtk_box_pack_start(GTK_BOX(profile_info), image, FALSE, FALSE, 0);
    add_class(image, "image");
    g_signal_connect(image, "clicked", G_CALLBACK(change_image), edit_window);

    GtkWidget *text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(profile_info), text_box, FALSE, FALSE, 0);

    GtkWidget *delete_button = create_image_button("Client/icons/trash.png", 20, 20);
    add_class(delete_button, "image");
    gtk_widget_set_halign(delete_button, GTK_ALIGN_END);
    gtk_widget_set_valign(delete_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), delete_button, TRUE, TRUE, 0);

    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_account), edit_window);

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

void chat_menu(GtkWidget *__attribute__((unused)) button)
{
    if (window_check == true)
        gtk_widget_destroy(edit_window);

    edit_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(edit_window), 300, 1);
    g_signal_connect(edit_window, "delete_event", G_CALLBACK(close_window), edit_window);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(edit_window), box);

    GtkWidget *chat_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), chat_info, FALSE, FALSE, 0);

    GtkWidget *edit_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), edit_box, FALSE, FALSE, 0);
    gtk_widget_set_name(edit_box, "edit");

    GtkWidget *image = create_image_button(access(account->current_chat->image_path, F_OK) == 0 ? account->current_chat->image_path : get_user_image(account->current_chat->image_id, account->current_chat->chat_type), 60, 60);
    gtk_widget_set_size_request(image, 60, 60);
    gtk_widget_set_valign(image, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(chat_info), image, FALSE, FALSE, 0);
    add_class(image, "image");
    g_signal_connect(image, "clicked", G_CALLBACK(change_chat_image), NULL);

    GtkWidget *text_box = gtk_box_new(account->current_chat->chat_type == CHAT_SAVED ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(chat_info), text_box, FALSE, FALSE, 0);

    // GtkWidget *delete_button = create_image_button("Client/icons/trash.png", 20, 20);
    // add_class(delete_button, "image");
    // gtk_widget_set_halign(delete_button, GTK_ALIGN_END);
    // gtk_widget_set_valign(delete_button, GTK_ALIGN_CENTER);
    // gtk_box_pack_start(GTK_BOX(box), delete_button, TRUE, TRUE, 0);

    // g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_chat), edit_window);

    if (account->current_chat->user_privilege == PRIV_ADMIN)
    {
        GtkWidget *delete_button = create_image_button("Client/icons/trash.png", 20, 20);
        add_class(delete_button, "image");
        gtk_widget_set_halign(delete_button, GTK_ALIGN_END);
        gtk_widget_set_valign(delete_button, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(box), delete_button, FALSE, FALSE, 0);

        g_signal_connect(delete_button, "clicked", G_CALLBACK(confirm_window), NULL);
    }

    GtkWidget *name = gtk_text_view_new();
    add_class(text_box, "edit");
    gtk_widget_set_name(name, "chatname");
    gtk_widget_set_halign(name, GTK_ALIGN_START);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(name), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(name), FALSE);
    gtk_box_pack_start(GTK_BOX(text_box), name, FALSE, FALSE, 0);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(name));
    gtk_text_buffer_set_text(buffer, account->current_chat->name, -1);

    if (account->current_chat->chat_type == CHAT_SAVED)
    {
        gtk_widget_show_all(edit_window);
        window_check = true;
        return;
    }
    char *users_count_text = mx_strjoin("users: ", mx_itoa(account->current_chat->users_count));

    GtkWidget *user_count = gtk_label_new(users_count_text);

    mx_strdel(&users_count_text);
    add_class(user_count, "count");
    gtk_widget_set_halign(user_count, GTK_ALIGN_START);
    gtk_widget_set_valign(user_count, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(text_box), user_count, FALSE, FALSE, 0);

    GtkWidget *edit_profile_box = gtk_event_box_new();
    gtk_box_pack_start(GTK_BOX(text_box), edit_profile_box, FALSE, FALSE, 10);
    GtkWidget *edit_profile = gtk_label_new(NULL);
    gtk_widget_set_halign(edit_profile, GTK_ALIGN_START);
    gtk_label_set_markup(GTK_LABEL(edit_profile), "<u>Change Name</u>");
    gtk_container_add(GTK_CONTAINER(edit_profile_box), edit_profile);
    add_class(edit_profile, "links");

    g_signal_connect(edit_profile_box, "button_press_event", G_CALLBACK(edit_chatname), NULL);

    // users
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, 400, 300);
    add_class(scrolled_window, "users-list");
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_widget_set_valign(scrolled_window, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    GtkWidget *box_for_users = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(scrolled_window), box_for_users);
    gtk_widget_set_name(box_for_users, "box_for_users");

    chat_users(edit_window);

    gtk_widget_show_all(edit_window);
    window_check = true;
}

void edit_chatname()
{
    GtkWidget *box = get_widget_by_name_r(edit_window, "edit");
    clear_box(box);

    GtkWidget *username_label = gtk_label_new("New chat name:");
    gtk_widget_set_valign(username_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), username_label, FALSE, FALSE, 0);

    GtkWidget *name_entry = gtk_entry_new();
    gtk_widget_set_name(name_entry, "chatname_entry");
    gtk_entry_set_text(GTK_ENTRY(name_entry), account->current_chat->name);
    gtk_box_pack_start(GTK_BOX(box), name_entry, FALSE, FALSE, 0);

    GtkWidget *accept_button = gtk_button_new_with_label("Accept");
    gtk_box_pack_start(GTK_BOX(box), accept_button, FALSE, FALSE, 0);
    g_signal_connect(accept_button, "clicked", G_CALLBACK(chat_accept_clicked), edit_window);
    gtk_widget_show_all(box);
}
