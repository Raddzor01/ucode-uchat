#include "../inc/client.h"

guint cancel_handler_id;
guint accept_handler_id;
guint cancel_entry_id;
guint accept_entry_id;
bool username_display;

void change_chat_id(GtkWidget *widget, gpointer user_data);

void load_css()
{
    GtkCssProvider *styles = gtk_css_provider_new();
    gtk_css_provider_load_from_path(styles, "Client/src/style/main.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(styles), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void add_class(GtkWidget *widget, char *class_name)
{
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(context, class_name);
}

void clear_window(GtkWidget *window)
{
    // Get the container widget of the window
    GtkWidget *container = gtk_bin_get_child(GTK_BIN(window));

    // Remove any existing widget from the container
    if (GTK_IS_WIDGET(container))
        gtk_container_remove(GTK_CONTAINER(window), container);
}

void clear_box(GtkWidget *box)
{

    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(box));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
    {
        gtk_container_remove(GTK_CONTAINER(box), GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

void create_new_window(char *title, int width, int height, bool resizable)
{
    if (!main_window)
    {
        main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 0);
        g_signal_connect(main_window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);
    }

    gtk_window_set_title(GTK_WINDOW(main_window), title);
    gtk_window_resize(GTK_WINDOW(main_window), width, height);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(main_window), resizable);
}

GtkWidget *create_image_button(char *image_path, int width, int height)
{
    GdkPixbuf *pixbuf;
    GtkWidget *button, *image;

    // Load and scale the image
    pixbuf = gdk_pixbuf_new_from_file(image_path, NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);

    // Create the image and button widgets
    image = gtk_image_new_from_pixbuf(pixbuf);
    button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), image);

    // Free the pixbuf
    g_object_unref(pixbuf);

    // Return the button widget
    return button;
}

GtkWidget *get_widget_by_name_r(GtkWidget *container, char *name)
{
    GtkWidget *result = NULL;
    GList *children = NULL;

    if (GTK_IS_CONTAINER(container))
    {
        children = gtk_container_get_children(GTK_CONTAINER(container));
    }
    else
    {
        return NULL;
    }

    while (children)
    {
        if (!mx_strcmp(gtk_widget_get_name(GTK_WIDGET(children->data)), name))
        {
            result = GTK_WIDGET(children->data);
            break;
        }
        else if (GTK_IS_CONTAINER(children->data))
        {
            result = get_widget_by_name_r(children->data, name);

            if (result != NULL)
                break;
        }

        children = children->next;
    }

    g_list_free(g_steal_pointer(&children)); // g_list_free(children_c); //

    return result;
}

void file_select(GtkWidget *widget, gpointer data)
{

    if (data)
    {
    };
    (void)widget;

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File", NULL, action, "_Cancel",
                                         GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        // send_file_to_server(filename);

        // Do something with the selected file...
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void user_box(t_chat *chat, bool is_search)
{
    GtkWidget *button;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *box;
    GtkWidget *out_box = get_widget_by_name_r(main_window, "box_for_users");

    // int *id_link = &account->chat_id_list[number];

    GdkPixbuf *pixbuf;
    GError *error = NULL;

    // Load the image from a file
    pixbuf = gdk_pixbuf_new_from_file("Client/Ass/HOG.png", &error);
    if (error != NULL)
        g_error("Error loading image: %s", error->message);

    // Scale the image to a new size
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);

    // Create a new GtkImage widget from the scaled image
    image = gtk_image_new_from_pixbuf(pixbuf);

    // create a label
    label = gtk_label_new(chat->name);

    // create a box to hold the image and label
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    g_object_unref(pixbuf);

    // create a button
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), box);
    gtk_box_pack_start(GTK_BOX(out_box), button, FALSE, FALSE, 0);
    gtk_widget_show_all(out_box);
    if (is_search) {
        g_signal_connect(button, "clicked", G_CALLBACK(join_chat), (gpointer)chat);
        // g_signal_connect(button, "clicked", G_CALLBACK(change_chat_id), (gpointer)chat);
    }else
        g_signal_connect(button, "clicked", G_CALLBACK(change_chat_id), (gpointer)chat);
}

void change_chat_id(GtkWidget *__attribute__((unused)) widget, gpointer user_data)
{
    build_chat_window();
    account->current_chat = (t_chat *)user_data;

    printf("\ncurrent chat: %d\n", account->current_chat->id);

    GtkWidget *chat = get_widget_by_name_r(main_window, "box_holder");
    clear_box(chat);

    t_msg *msg = account->current_chat->messages;
    while (msg != NULL)
    {
        if (msg->user_id == account->id)
            text_bubble(msg->text, msg->msg_id);
        else
            receive_bubble(msg->text, msg->username);
        msg = msg->next;
    }
}

void receive_bubble(const char *text, const char *name)
{
    GtkWidget *box_container = get_widget_by_name_r(main_window, "box_holder");

    GtkWidget *username_box;
    GtkWidget *username;
    GtkWidget *text_view;
    GtkWidget *box;
    GtkTextBuffer *buffer;
    GtkWidget *time_label;

    char *time = {"16:30"};

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_START);

    // username
    if (username_display)
    {
        GdkPixbuf *pixbuf;
        GtkWidget *image;

        pixbuf = gdk_pixbuf_new_from_file("Client/Ass/HOG.png", NULL);
        pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);
        image = gtk_image_new_from_pixbuf(pixbuf);
        g_object_unref(pixbuf);

        gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    }
    else
    {
        GtkWidget *blank_space = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_set_size_request(blank_space, 50, -1);
        gtk_box_pack_start(GTK_BOX(box), blank_space, FALSE, FALSE, 0);
    }

    // text bubble
    username_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    add_class(username_box, "receive");
    if (username_display)
    {
        username = gtk_label_new(name);
        add_class(username, "username");
        gtk_widget_set_halign(username, GTK_ALIGN_START);
        gtk_widget_set_valign(username, GTK_ALIGN_END);
        gtk_box_pack_start(GTK_BOX(username_box), username, FALSE, FALSE, 0);
    }
    gtk_widget_set_halign(username_box, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), username_box, FALSE, FALSE, 0);

    // text
    text_view = gtk_text_view_new();

    gtk_widget_set_valign(text_view, GTK_ALIGN_END);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_box_pack_start(GTK_BOX(username_box), text_view, FALSE, TRUE, 0);

    PangoLayout *layout = gtk_widget_create_pango_layout(text_view, text);
    int width, height;
    pango_layout_get_pixel_size(layout, &width, &height);

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_NONE);

    if (width > 400)
    {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
        gtk_widget_set_size_request(text_view, 400, -1);
        gtk_widget_set_hexpand(text_view, TRUE);
        gtk_widget_set_halign(text_view, GTK_ALIGN_FILL);
        gtk_widget_set_hexpand(username_box, TRUE);
        gtk_widget_set_halign(username_box, GTK_ALIGN_START);
    }

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, text, strlen(text));

    // show time
    time_label = gtk_label_new(time);
    gtk_widget_set_halign(time_label, GTK_ALIGN_END);
    gtk_widget_set_valign(time_label, GTK_ALIGN_START);
    add_class(time_label, "time");
    gtk_box_pack_start(GTK_BOX(username_box), time_label, FALSE, TRUE, 0);

    gtk_widget_show_all(box_container);
    gtk_widget_queue_draw(main_window);
    username_display = FALSE;
}

void text_bubble(const char *text, int msg_id)
{
    username_display = TRUE;
    GtkWidget *box_container = get_widget_by_name_r(main_window, "box_holder");
    GtkWidget *scrolled_window = get_widget_by_name_r(main_window, "scroll");
    GtkWidget *box;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;
    GtkWidget *time_box;
    GtkWidget *time_label;

    char *time = {"16:30"};

    // text part
    gtk_widget_set_hexpand(box_container, FALSE);

    // box with text and buttons
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_halign(box, GTK_ALIGN_END);
    gtk_widget_set_size_request(box, -1, 30);
    add_class(box, "bubble");
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_vexpand(box, FALSE);
    gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);

    time_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand(time_box, FALSE);
    gtk_widget_set_vexpand(time_box, FALSE);
    gtk_widget_set_halign(time_box, GTK_ALIGN_END);
    gtk_box_pack_start(GTK_BOX(box), time_box, FALSE, FALSE, 0);

    text_view = gtk_text_view_new();

    gtk_widget_set_valign(text_view, GTK_ALIGN_CENTER);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_container_add(GTK_CONTAINER(time_box), text_view);

    PangoLayout *layout = gtk_widget_create_pango_layout(text_view, text);
    int width, height;
    pango_layout_get_pixel_size(layout, &width, &height);

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_NONE);

    if (width > 400)
    {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
        gtk_widget_set_size_request(text_view, 400, -1);
        gtk_widget_set_hexpand(text_view, TRUE);
        gtk_widget_set_halign(text_view, GTK_ALIGN_END);

        gtk_widget_set_hexpand(time_box, TRUE);
        gtk_widget_set_halign(time_box, GTK_ALIGN_END);
    }

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, text, strlen(text));

    // show time
    time_label = gtk_label_new(time);
    gtk_widget_set_halign(time_label, GTK_ALIGN_START);
    gtk_widget_set_valign(time_label, GTK_ALIGN_START);
    add_class(time_label, "time");
    gtk_box_pack_start(GTK_BOX(time_box), time_label, FALSE, FALSE, 0);

    // gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);

    // redact button
    GtkWidget *edit_button;

    edit_button = create_image_button("Client/icons/pen.png", 12, 12);
    gtk_widget_set_vexpand(edit_button, FALSE);
    gtk_widget_set_valign(edit_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), edit_button, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(change_msg_id_for_edit), GINT_TO_POINTER(msg_id));
    g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(edit_msg), text_view);

    // delete button
    GtkWidget *delete_button;

    delete_button = create_image_button("Client/icons/trash.png", 12, 12);
    gtk_widget_set_vexpand(delete_button, FALSE);
    gtk_widget_set_valign(delete_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), delete_button, FALSE, FALSE, 0);

    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_msg_id), GINT_TO_POINTER(msg_id));
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_msg), box);

    // scroll to the bottom
    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
    gtk_adjustment_set_value(adjustment, gtk_adjustment_get_upper(adjustment) - gtk_adjustment_get_page_size(adjustment));

    gtk_widget_show_all(box_container);
    gtk_widget_queue_draw(main_window);
}

void delete_msg(GtkButton *__attribute__((unused)) button, gpointer data)
{
    GtkWidget *box = (GtkWidget *)data;
    gtk_widget_destroy(box);
}

void delete_msg_id(GtkButton *__attribute__((unused)) button, gpointer msg_id)
{
    delete_msg_in_server(GPOINTER_TO_INT(msg_id));
}

void cancel_edit(GtkButton *__attribute__((unused)) button, gpointer data)
{
    GtkWidget *send_button = get_widget_by_name_r(main_window, "send_button");
    GtkWidget *entry = get_widget_by_name_r(main_window, "chat_text_entry");

    g_signal_handler_disconnect(G_OBJECT(send_button), cancel_handler_id);
    g_signal_handler_disconnect(G_OBJECT(send_button), accept_handler_id);
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), NULL);

    g_signal_handler_disconnect(G_OBJECT(entry), cancel_entry_id);
    g_signal_handler_disconnect(G_OBJECT(entry), accept_entry_id);
    g_signal_connect(entry, "activate", G_CALLBACK(send_message), NULL);

    gtk_entry_set_text(GTK_ENTRY(info->entry), "");

    GtkWidget *box = (GtkWidget *)data;
    gtk_widget_destroy(box);
}

void edit_msg(GtkButton *__attribute__((unused)) button, gpointer data)
{
    GtkTextView *text_view = GTK_TEXT_VIEW(data);
    GtkWidget *chat_box = get_widget_by_name_r(main_window, "chat_box");
    GtkWidget *cancel_button;
    GtkWidget *edit_text;
    GtkWidget *entry = get_widget_by_name_r(main_window, "chat_text_entry");
    GtkWidget *send_button = get_widget_by_name_r(main_window, "send_button");

    if(get_widget_by_name_r(main_window, "edit")) {
        GtkWidget *temp = get_widget_by_name_r(main_window, "edit");
        gtk_widget_destroy(temp);

        g_signal_handler_disconnect(G_OBJECT(send_button), cancel_handler_id);
        g_signal_handler_disconnect(G_OBJECT(send_button), accept_handler_id);
        g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), NULL);

        g_signal_handler_disconnect(G_OBJECT(entry), cancel_entry_id);
        g_signal_handler_disconnect(G_OBJECT(entry), accept_entry_id);
        g_signal_connect(entry, "activate", G_CALLBACK(send_message), NULL);

        gtk_entry_set_text (GTK_ENTRY (info->entry), "");
    }

    //take text from bubble (to set it to entry)
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    //create redact field
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_size_request(box, -1, -1);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(chat_box), box, FALSE, FALSE, 0);
    gtk_widget_set_name(box, "edit");
    add_class(box, "edit");

    //edited text
    edit_text = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(edit_text), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(edit_text), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(edit_text), FALSE);
    gtk_widget_set_size_request(edit_text, 400, -1);
    gtk_widget_set_hexpand(edit_text, TRUE);
    gtk_widget_set_halign(edit_text, GTK_ALIGN_FILL);
    gtk_widget_set_valign(edit_text, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), edit_text, FALSE, FALSE, 0);

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(edit_text));
    gtk_text_buffer_set_text(buffer, text, strlen(text));

    //close edit tab

    cancel_button = create_image_button("Client/icons/cross.png", 14, 14);
    gtk_widget_set_hexpand(cancel_button, TRUE);
    gtk_widget_set_halign(cancel_button, GTK_ALIGN_END);
    gtk_box_pack_start(GTK_BOX(box), cancel_button, FALSE, FALSE, 0);

    g_signal_handlers_disconnect_by_func(send_button, (gpointer)send_message, NULL);
    accept_handler_id = g_signal_connect(G_OBJECT(send_button), "clicked", G_CALLBACK(edit_accept), text_view);
    cancel_handler_id = g_signal_connect(G_OBJECT(send_button), "clicked", G_CALLBACK(cancel_edit), box);

    g_signal_handlers_disconnect_by_func(entry, (gpointer)send_message, NULL);
    accept_entry_id = g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(edit_accept), text_view);
    cancel_entry_id = g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(cancel_edit), box);

    g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(cancel_edit), box);

    gtk_widget_show_all(chat_box);
}

void edit_accept(GtkButton *__attribute__((unused)) button, gpointer data)
{
    GtkWidget *text_view = GTK_WIDGET(data);
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(info->entry));

    if (strlen(text) == 0)
        return;

    PangoLayout *layout = gtk_widget_create_pango_layout(text_view, text);
    int width, height;
    pango_layout_get_pixel_size(layout, &width, &height);

    if (width > 400)
    {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
        gtk_widget_set_size_request(text_view, 400, -1);
        gtk_widget_set_halign(text_view, GTK_ALIGN_END);
    }
    else
    {
        gtk_widget_set_size_request(text_view, width, -1);
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_NONE);
        gtk_widget_set_halign(text_view, GTK_ALIGN_START);
    }

    g_object_unref(layout);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    gtk_text_buffer_set_text(buffer, text, strlen(text));
    edit_msg_in_server(info->msg_id_for_edit, text);
}

void change_msg_id_for_edit(GtkButton *__attribute__((unused)) button, gpointer msg_id)
{
    info->msg_id_for_edit = GPOINTER_TO_INT(msg_id);
}

void create_chat(GtkButton *__attribute__((unused)) button, gpointer chatname)
{
    const char *text = gtk_entry_get_text(GTK_ENTRY(GTK_ENTRY(chatname)));

    create_chat_in_server(text, CHAT_NORMAL);

    // int chat_id = check_chat_id_from_server();
}

void join_chat(GtkWidget *__attribute__((unused)) widget, gpointer user_data)
{
    t_chat *chat = (t_chat *)user_data;
    chat_push_back(&account->chats, chat);
    join_to_found_chat(chat->id);
}
