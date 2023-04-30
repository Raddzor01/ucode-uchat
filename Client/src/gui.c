#include "../inc/client.h"

guint cancel_handler_id;
guint accept_handler_id;
guint cancel_entry_id;
guint accept_entry_id;
bool username_display = TRUE;
extern GtkWidget *edit_window;

void change_chat_id(GtkWidget *widget, gpointer user_data);

void load_css()
{
    GtkCssProvider *styles = gtk_css_provider_new();
    gtk_css_provider_load_from_path(styles, "Client/src/style/main.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(styles), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void add_class(GtkWidget *widget, char *name)
{
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(context, name);
}

void clear_window(GtkWidget *window)
{
    GtkWidget *container = gtk_bin_get_child(GTK_BIN(window));

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

    pixbuf = gdk_pixbuf_new_from_file(image_path, NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);

    image = gtk_image_new_from_pixbuf(pixbuf);
    button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(button), image);

    g_object_unref(pixbuf);

    return button;
}

GtkWidget *get_widget_by_name_r(GtkWidget *container, char *name)
{
    GtkWidget *result = NULL;
    GList *children = NULL;

    if (GTK_IS_CONTAINER(container))
        children = gtk_container_get_children(GTK_CONTAINER(container));
    else
        return NULL;

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

        send_file_to_server(filename);

        // Do something with the selected file...
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void last_massage_display(char *chatname, char *message)
{
    GtkWidget *box = get_widget_by_name_r(main_window, chatname);
    clear_box(box);

    GtkWidget *label = gtk_label_new(message);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    add_class(label, "last_message");
    gtk_widget_show_all(box);
}

void user_box(t_chat *chat, bool is_search)
{
    GtkWidget *button;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *box;
    GtkWidget *out_box = get_widget_by_name_r(main_window, "box_for_users");

    if (get_widget_by_name_r(main_window, "empty_left_bar"))
        gtk_widget_destroy(get_widget_by_name_r(main_window, "empty_left_bar"));

    GdkPixbuf *pixbuf;
    GError *error = NULL;

    char *chat_name = str_to_display_chat_name(chat->name);
    char *last_msg = NULL;

    // pixbuf = gdk_pixbuf_new_from_file(get_image_from_server(chat->id), &error);
    pixbuf = gdk_pixbuf_new_from_file(access(chat->image_path, F_OK) == 0 ? chat->image_path : get_user_image(chat->image_id, chat->chat_type), &error);
    if (error != NULL)
        g_error("Error loading image: %s", error->message);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(pixbuf);

    label = gtk_label_new(chat_name);
    gtk_widget_set_halign(label, GTK_ALIGN_START);

    GtkWidget *text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(text_box), label, FALSE, FALSE, 0);

    GtkWidget *last_message = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(text_box), last_message, FALSE, FALSE, 0);
    gtk_widget_set_name(last_message, chat->name);
    t_msg *temp = msg_get_last_message(chat->messages);
    if (temp == NULL)
        last_msg = mx_strdup("No messages yet");
    else
        last_msg = str_to_display_last_msg(temp->text, temp->username);
    GtkWidget *last = gtk_label_new(last_msg);
    gtk_box_pack_start(GTK_BOX(last_message), last, FALSE, FALSE, 0);
    add_class(last, "last_message");

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), text_box, FALSE, FALSE, 0);

    g_object_unref(pixbuf);

    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), box);
    gtk_box_pack_start(GTK_BOX(out_box), button, FALSE, FALSE, 0);
    gtk_widget_show_all(out_box);
    if (is_search)
        g_signal_connect(button, "clicked", G_CALLBACK(join_chat), (gpointer)chat);
    g_signal_connect(button, "clicked", G_CALLBACK(change_chat_id), (gpointer)chat);

    free(last_msg);
    free(chat_name);
}

void change_chat_id(GtkWidget *__attribute__((unused)) widget, gpointer user_data)
{
    build_chat_window();
    account->current_chat = (t_chat *)user_data;
    chat_info();
    username_display = TRUE;

    GtkWidget *chat = get_widget_by_name_r(main_window, "box_holder");
    clear_box(chat);

    t_msg *message = account->current_chat->messages;
    while (message != NULL)
    {
        if (message->user_id == account->id)
            text_bubble(message);
        else
            receive_bubble(message);
        message = message->next;
    }

    // GtkWidget *scrolled_window = get_widget_by_name_r(main_window, "scroll");
    // GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
    // gtk_adjustment_set_value(adjustment, gtk_adjustment_get_upper(adjustment) - gtk_adjustment_get_page_size(adjustment));

    gtk_widget_show_all(chat);
}

void receive_bubble(t_msg *message)
{
    GtkWidget *box_container = get_widget_by_name_r(main_window, "box_holder");

    GtkWidget *username_box;
    GtkWidget *username;
    GtkWidget *text_view;
    GtkWidget *box;
    GtkTextBuffer *buffer;
    GtkWidget *time_label;

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    char *msg_name = mx_strjoin(MSG_NAME, mx_itoa(message->msg_id));
    gtk_widget_set_name(box, msg_name);
    mx_strdel(&msg_name);
    gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_START);

    // username
    if (username_display)
    {
        GdkPixbuf *pixbuf;
        GtkWidget *image;

        pixbuf = gdk_pixbuf_new_from_file(get_user_image(message->image_id, PFP_USER), NULL);
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

    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), button_box, FALSE, FALSE, 0);
    username_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    add_class(button_box, "receive");
    if (username_display)
    {
        username = gtk_label_new(message->username);
        add_class(username, "username");
        gtk_widget_set_halign(username, GTK_ALIGN_START);
        gtk_widget_set_valign(username, GTK_ALIGN_END);
        gtk_box_pack_start(GTK_BOX(username_box), username, FALSE, FALSE, 0);
    }
    gtk_widget_set_halign(username_box, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(button_box), username_box, FALSE, FALSE, 0);

    // text
    text_view = gtk_text_view_new();

    gtk_widget_set_name(text_view, "message_text_view");

    gtk_widget_set_valign(text_view, GTK_ALIGN_END);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_box_pack_start(GTK_BOX(username_box), text_view, FALSE, TRUE, 0);

    PangoLayout *layout = gtk_widget_create_pango_layout(text_view, message->text);
    int width, height;
    pango_layout_get_pixel_size(layout, &width, &height);

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_NONE);

    if (width > 400)
    {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
        gtk_widget_set_size_request(text_view, 400, -1);
        gtk_widget_set_hexpand(text_view, TRUE);
        gtk_widget_set_halign(text_view, GTK_ALIGN_FILL);
        gtk_widget_set_hexpand(username_box, TRUE);
        gtk_widget_set_halign(username_box, GTK_ALIGN_START);
    }

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, message->text, mx_strlen(message->text));

    // show time
    time_label = gtk_label_new(get_send_time_str(message->time));
    gtk_widget_set_halign(time_label, GTK_ALIGN_END);
    gtk_widget_set_valign(time_label, GTK_ALIGN_START);
    add_class(time_label, "time");
    gtk_box_pack_start(GTK_BOX(username_box), time_label, FALSE, TRUE, 0);

    if ((account->current_chat->user_privilege == PRIV_ADMIN) || (account->current_chat->user_privilege == PRIV_MODERATOR))
    {
        GtkWidget *delete_button;

        delete_button = create_image_button("Client/icons/trash.png", 12, 12);
        add_class(delete_button, "bubble_button");
        gtk_widget_set_vexpand(delete_button, FALSE);
        gtk_widget_set_valign(delete_button, GTK_ALIGN_CENTER);
        gtk_box_pack_start(GTK_BOX(button_box), delete_button, FALSE, FALSE, 0);

        g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_msg_id), GINT_TO_POINTER(message->msg_id));
        g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_msg), box);
    }

    gtk_widget_show_all(box_container);
    gtk_widget_queue_draw(main_window);
    username_display = FALSE;
}

void text_bubble(t_msg *message)
{
    username_display = TRUE;
    GtkWidget *box_container = get_widget_by_name_r(main_window, "box_holder");
    GtkWidget *scrolled_window = get_widget_by_name_r(main_window, "scroll");
    GtkWidget *box;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;
    GtkWidget *time_box;
    GtkWidget *time_label;

    // text part
    gtk_widget_set_hexpand(box_container, FALSE);

    // box with text and buttons
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_halign(box, GTK_ALIGN_END);
    gtk_widget_set_size_request(box, -1, 30);
    char *msg_name = mx_strjoin(MSG_NAME, mx_itoa(message->msg_id));
    gtk_widget_set_name(box, msg_name);
    mx_strdel(&msg_name);
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

    PangoLayout *layout = gtk_widget_create_pango_layout(text_view, message->text);
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
    gtk_text_buffer_set_text(buffer, message->text, mx_strlen(message->text));

    // show time
    time_label = gtk_label_new(get_send_time_str(message->time));
    gtk_widget_set_halign(time_label, GTK_ALIGN_START);
    gtk_widget_set_valign(time_label, GTK_ALIGN_START);
    add_class(time_label, "time");
    gtk_box_pack_start(GTK_BOX(time_box), time_label, FALSE, FALSE, 0);

    // gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);

    // redact button
    GtkWidget *edit_button;

    edit_button = create_image_button("Client/icons/pen.png", 12, 12);
    add_class(edit_button, "bubble_button");
    gtk_widget_set_vexpand(edit_button, FALSE);
    gtk_widget_set_valign(edit_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), edit_button, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(change_msg_id_for_edit), GINT_TO_POINTER(message->msg_id));
    g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(edit_msg), text_view);

    // delete button
    GtkWidget *delete_button;

    delete_button = create_image_button("Client/icons/trash.png", 12, 12);
    add_class(delete_button, "bubble_button");
    gtk_widget_set_vexpand(delete_button, FALSE);
    gtk_widget_set_valign(delete_button, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), delete_button, FALSE, FALSE, 0);

    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_msg_id), GINT_TO_POINTER(message->msg_id));
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

void delete_msg_id(GtkButton *__attribute__((unused)) button, gpointer data)
{
    int message_id = GPOINTER_TO_INT(data);
    delete_msg_in_server(message_id);
    pthread_mutex_lock(&account->mutex);
    msg_pop_by_message_id(&account->current_chat->messages, message_id);
    pthread_mutex_unlock(&account->mutex);

    char *last_msg_str = str_to_display_last_msg((msg_list_size(account->current_chat->messages) != 0) ? msg_get_last_message(account->current_chat->messages)->text : mx_strdup("No message yet"), account->username);
    last_massage_display(account->current_chat->name, last_msg_str);
    mx_strdel(&last_msg_str);
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

    if (get_widget_by_name_r(main_window, "edit"))
    {
        GtkWidget *temp = get_widget_by_name_r(main_window, "edit");
        gtk_widget_destroy(temp);

        g_signal_handler_disconnect(G_OBJECT(send_button), cancel_handler_id);
        g_signal_handler_disconnect(G_OBJECT(send_button), accept_handler_id);
        g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), NULL);

        g_signal_handler_disconnect(G_OBJECT(entry), cancel_entry_id);
        g_signal_handler_disconnect(G_OBJECT(entry), accept_entry_id);
        g_signal_connect(entry, "activate", G_CALLBACK(send_message), NULL);

        gtk_entry_set_text(GTK_ENTRY(info->entry), "");
    }

    // take text from bubble (to set it to entry)
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    // create redact field
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_size_request(box, -1, -1);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(chat_box), box, FALSE, FALSE, 0);
    gtk_widget_set_name(box, "edit");
    add_class(box, "edit");

    // edited text
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

    // close edit tab

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

    if (mx_strlen(text) == 0)
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

void create_chat(GtkButton *__attribute__((unused)) button, GtkWidget *window)
{
    GtkWidget *box = get_widget_by_name_r(window, "chatname_box");
    GtkWidget *chatname_error_label = get_widget_by_name_r(box, "chatname_error_label");
    const char *text = gtk_entry_get_text(GTK_ENTRY(GTK_ENTRY(get_widget_by_name_r(box, "chatname_entry"))));

    if (!mx_strcmp(text, "") || !mx_strcmp(text, " "))
    {
        gtk_label_set_text(GTK_LABEL(chatname_error_label), "Forbidden chat name\nPlease try again");
        return;
    }

    int chat_id = create_chat_in_server(text, CHAT_NORMAL);

    if (chat_id == -1)
        return;

    t_chat *chat = chat_prepare_node(chat_id, text, 1, PRIV_ADMIN, CHAT_NORMAL);
    chat->image_path = mx_strdup(DEFAULT_CHAT_IMAGE);
    chat->users = malloc(sizeof(t_user *));
    chat->users[0] = (t_user *)malloc(sizeof(t_user));
    chat->users[0]->id = account->id;
    chat->users[0]->image_id = account->image_id;
    chat->users[0]->username = mx_strdup(account->username);
    chat->users[0]->privilege = PRIV_ADMIN;

    pthread_mutex_lock(&account->mutex);
    chat_push_front(&account->chats, chat);
    pthread_mutex_unlock(&account->mutex);

    close_window_by_button(button, (gpointer *)window);

    GtkWidget *box_for_users = get_widget_by_name_r(main_window, "box_for_users");
    clear_box(box_for_users);

    display_users();
}

void create_saved(GtkButton *__attribute__((unused)) button)
{
    char *chat_name = mx_strjoin(SAVED_NAME, account->username);

    int chat_id = create_chat_in_server(chat_name, CHAT_SAVED);

    if (chat_id == -1)
        return;

    t_chat *chat = chat_prepare_node(chat_id, "Saved messages", 1, PRIV_ADMIN, CHAT_SAVED);
    chat->image_path = mx_strdup(SAVED_IMAGE);
    chat->users = malloc(sizeof(t_user *));
    chat->users[0] = (t_user *)malloc(sizeof(t_user));
    chat->users[0]->id = account->id;
    chat->users[0]->image_id = account->image_id;
    chat->users[0]->username = mx_strdup(account->username);
    chat->users[0]->privilege = PRIV_ADMIN;

    pthread_mutex_lock(&account->mutex);
    chat_push_front(&account->chats, chat);
    pthread_mutex_unlock(&account->mutex);

    GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");
    clear_box(box);

    display_users();
}

void join_chat(GtkWidget *__attribute__((unused)) widget, gpointer user_data)
{
    t_chat *chat = (t_chat *)user_data;
    join_to_found_chat(chat->id);
    pthread_mutex_lock(&account->mutex);
    chat->messages = get_chat_messages_from_server(chat->id);
    chat_push_front(&account->chats, chat);
    pthread_mutex_unlock(&account->mutex);

    GtkWidget *search_entry = get_widget_by_name_r(main_window, "Search_entry");
    gtk_entry_set_text(GTK_ENTRY(search_entry), "");

    GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");
    clear_box(box);
    display_users();
}

char *get_send_time_str(time_t msg_time)
{
    char *result_str = NULL;
    char s[20];
    struct tm *time_struct = localtime(&msg_time);
    for (int i = 0; i < 20; i++)
        s[i] = 0;
    //   strftime(s, 40, "%d.%m.%Y %H:%M:%S, %A", time_struct);

    if (msg_time > info->current_day_time)
    {
        result_str = (char *)malloc(strftime(s, 20, "%H:%M", time_struct));
        mx_strcpy(result_str, s);
        return (result_str);
    }
    else if (msg_time < info->current_day_time)
    {
        result_str = (char *)malloc(strftime(s, 20, "%d.%m %H:%M", time_struct));
        mx_strcpy(result_str, s);
        return (result_str);
    }

    return result_str;
}

void accept_clicked(GtkButton *__attribute__((unused)) button, GtkWidget *window)
{
    GtkWidget *box = get_widget_by_name_r(window, "edit");
    GtkWidget *username_label = get_widget_by_name_r(window, "username");

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(get_widget_by_name_r(window, "username_entry")));

    const gchar *password = gtk_entry_get_text(GTK_ENTRY(get_widget_by_name_r(window, "password_entry")));

    if (strlen(username) && (strcmp(username, account->username) != 0))
    {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(username_label));

        gtk_text_buffer_set_text(buffer, username, -1);

        edit_username_in_server(username);
    }
    else if (!strlen(username))
        pop_up_window("New username must store at least one character");

    if (strlen(password))
        edit_password_in_server(password);

    if ((strcmp(username, account->username) != 0) && strlen(password))
        pop_up_window("You didn't change anything");

    clear_box(box);
    gtk_window_resize(GTK_WINDOW(window), 300, 1);
    gtk_widget_show_all(window);

    GtkWidget *widget_to_remove = gtk_grid_get_child_at(GTK_GRID(get_widget_by_name_r(main_window, "chat_grid")), 0, 0);
    gtk_container_remove(GTK_CONTAINER(get_widget_by_name_r(main_window, "chat_grid")), widget_to_remove);
    build_users();
}

int count_max_size_for_str(int size, const char *str)
{
    int result_size = size;
    int upper_case_count = 0;
    int i = 0;

    while (*str != '\0' && i < size)
    {
        if (mx_isupper(*str))
            upper_case_count++;

        if (upper_case_count == 7)
        {
            result_size--;
            upper_case_count = 0;
        }

        str++;
        i++;
    }

    return result_size;
}

char *str_to_display_last_msg(const char *msg, const char *username)
{
    char *result_str = NULL;
    int max_number_for_char = count_max_size_for_str(MAX_NUMBER_OF_CHAR_FOR_LAST_MSG, msg);

    if (mx_strcmp(username, account->username) == 0)
    {
        if ((mx_strlen(msg) + 5) < max_number_for_char)
        {
            result_str = mx_strnew(strlen(msg) + 5);
            sprintf(result_str, "You: %s", msg);
            return result_str;
        }
        else
        {
            result_str = mx_strnew(max_number_for_char);
            sprintf(result_str, "You: ");
            strncat(result_str, msg, (max_number_for_char - 7));
            mx_strcat(result_str, "...");
            // result_str = mx_strjoin(result_str, "...");
            return result_str;
        }
    }
    else
    {
        max_number_for_char = count_max_size_for_str(max_number_for_char, username);

        if ((mx_strlen(msg) + mx_strlen(username) + 2) < max_number_for_char)
        {
            result_str = mx_strnew(strlen(username) + strlen(msg) + 2);
            sprintf(result_str, "%s: %s", username, msg);
            return result_str;
        }
        else
        {
            result_str = mx_strnew(max_number_for_char);
            sprintf(result_str, "%s: ", username);
            strncat(result_str, msg, (max_number_for_char - strlen(result_str) - 2));
            mx_strcat(result_str, "...");
            // result_str = mx_strjoin(result_str, "...");
            return result_str;
        }
    }
}

char *str_to_display_chat_name(char *chat_name)
{
    char *result_str = NULL;
    int max_number_for_char = count_max_size_for_str(MAX_NUMBER_OF_CHAR_FOR_CHAT_NAME, chat_name);

    if (mx_strlen(chat_name) < max_number_for_char)
        return mx_strdup(chat_name);
    else
    {
        result_str = mx_strnew(max_number_for_char);
        strncat(result_str, chat_name, (max_number_for_char - 2));
        // mx_strjoin(result_str, "...");
        mx_strcat(result_str, "...");
        return result_str;
    }
}

void copy_image(char *original_path, char *new_path)
{
    FILE *original_file = fopen(original_path, "rb");
    FILE *new_file = fopen(new_path, "wb");

    if (original_file == NULL || new_file == NULL)
    {
        printf("Error: failed to open file.\n");
        return;
    }

    int c;
    while ((c = fgetc(original_file)) != EOF)
        fputc(c, new_file);

    fclose(original_file);
    fclose(new_file);
}

void change_chat_image(GtkWidget *__attribute__((unused)) button)
{
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Choose avatar", NULL, action, "_Cancel",
                                         GTK_RESPONSE_CANCEL, "_Select", GTK_RESPONSE_ACCEPT, NULL);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        time_t send_time = update_chat_photo(filename, account->current_chat->id);
        char *file_name = mx_strjoin(mx_ltoa(send_time), mx_itoa(account->id));
        char *file_with_extension = mx_strjoin(file_name, get_file_extension(filename));
        char *file_path = mx_strjoin(DATA_DIR, file_with_extension);

        copy_image(filename, file_path);

        pthread_mutex_lock(&account->mutex);
        mx_strdel(&account->current_chat->image_path);
        account->current_chat->image_path = mx_strdup(file_path);
        pthread_mutex_unlock(&account->mutex);

        mx_strdel(&file_name);
        mx_strdel(&file_with_extension);
        mx_strdel(&file_path);

        chat_info();
        GtkWidget *box = get_widget_by_name_r(main_window, "box_for_users");

        clear_box(box);
        display_users();
        chat_menu(NULL);
    }

    gtk_widget_destroy(dialog);
}

void change_image(GtkWidget *button)
{
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Choose avatar", NULL, action, "_Cancel",
                                         GTK_RESPONSE_CANCEL, "_Select", GTK_RESPONSE_ACCEPT, NULL);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        time_t send_time = send_file_to_server(filename);
        char *file_name = mx_strjoin(mx_ltoa(send_time), mx_itoa(account->id));
        char *file_with_extension = mx_strjoin(file_name, get_file_extension(filename));
        char *file_path = mx_strjoin(DATA_DIR, file_with_extension);

        copy_image(filename, file_path);

        pthread_mutex_lock(&account->mutex);
        mx_strdel(&account->image_path);
        account->image_path = mx_strdup(file_path);
        pthread_mutex_unlock(&account->mutex);

        mx_strdel(&file_name);
        mx_strdel(&file_with_extension);
        mx_strdel(&file_path);

        GtkWidget *widget_to_remove = gtk_grid_get_child_at(GTK_GRID(get_widget_by_name_r(main_window, "chat_grid")), 0, 0);
        gtk_container_remove(GTK_CONTAINER(get_widget_by_name_r(main_window, "chat_grid")), widget_to_remove);
        build_users();
        GdkPixbuf *pixbuf;

        pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        pixbuf = gdk_pixbuf_scale_simple(pixbuf, 60, 60, GDK_INTERP_BILINEAR);

        GtkWidget *new_image = gtk_image_new_from_pixbuf(pixbuf);
        gtk_button_set_image(GTK_BUTTON(button), new_image);
        g_free(filename);
        gtk_widget_queue_draw(main_window);
        g_object_unref(pixbuf);
    }

    gtk_widget_destroy(dialog);
}

void delete_account(GtkWidget *__attribute__((unused)) button, GtkWidget *window)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(main_window), GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
                                    "Are you sure?");
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_YES)
    {
        delete_account_in_server();
        // send_logout_to_server();
        log_menu(NULL);
        gtk_widget_destroy(window);
    }
    else if (result == GTK_RESPONSE_NO)
    {
        pop_up_window("Something went wrong\nTry again");
    }
    gtk_widget_destroy(dialog);
}

bool check_str_for_spec_char(const char *str)
{
    if (!str)
        return false;

    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] < 48 || (str[i] > 57 && str[i] < 65) || (str[i] > 90 && str[i] < 97) || (str[i] > 122 && str[i] < 127))
            return false;

    return true;
}

int is_box_empty(GtkBox *box)
{
    GList *children = gtk_container_get_children(GTK_CONTAINER(box));
    int is_empty = g_list_length(children) == 0;
    g_list_free(children);
    return is_empty;
}

void empty_left_bar()
{
    GtkWidget *out_box = get_widget_by_name_r(main_window, "box_for_users");

    if (!is_box_empty(GTK_BOX(out_box)))
        return;

    GtkWidget *label = gtk_label_new("Don't have chat?\nJoin or create one");
    gtk_widget_set_name(label, "empty_left_bar");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(out_box), label, FALSE, FALSE, 0);
}

void empty_right_bar()
{

    GtkWidget *grid = get_widget_by_name_r(main_window, "chat_grid");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(box, "empty_right_bar");
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(box, GTK_ALIGN_FILL);
    gtk_grid_attach(GTK_GRID(grid), box, 1, 0, 1, 1);
    gtk_widget_set_size_request(box, 500, -1);

    GtkWidget *label = gtk_label_new("Select a chat to start messaging");
    gtk_widget_set_vexpand(label, TRUE);
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
}

void chat_users(GtkWidget *window)
{
    for (int i = 0; account->current_chat->users[i] != NULL; i++)
        chat_user_box(window, account->current_chat->users[i]);
}

void kick_user(GtkWidget *__attribute__((unused)) button, gpointer data)
{
    t_user *user = (t_user *)data;
    int error_code = kick_user_in_server(account->current_chat->id, user->id);
    if (error_code != ERR_SUCCESS)
    {
        if (error_code == ERR_USER_DONT_HAVE_PERMISSION)
            pop_up_window("You don't have permission to kick this user");
        if (error_code == ERR_USER_NOT_IN_CHAT)
            pop_up_window("This user is not in this chat");
        return;
    }
    char *user_name = mx_strjoin(USER_NAME, mx_itoa(user->id));
    GtkWidget *user_box = get_widget_by_name_r(edit_window, user_name);
    mx_strdel(&user_name);
    gtk_widget_destroy(user_box);
    gtk_widget_show_all(edit_window);
}

char *get_user_privilege_str(t_privilege_type privilege)
{
    switch (privilege)
    {
    case PRIV_ADMIN:
        return "Admin";
    
    case PRIV_MODERATOR:
        return "Moderator";
    default:
        return "User";
    }
}

void chat_user_box(GtkWidget *window, t_user *user)
{
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *box;
    GtkWidget *out_box = get_widget_by_name_r(window, "box_for_users");

    GdkPixbuf *pixbuf;
    GError *error = NULL;

    // pixbuf = gdk_pixbuf_new_from_file(get_image_from_server(chat->id), &error);
    pixbuf = gdk_pixbuf_new_from_file(get_user_image(user->image_id, PFP_USER), &error);
    if (error != NULL)
        g_error("Error loading image: %s", error->message);

    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);
    image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_widget_set_name(image, "chat_menu_image");

    label = gtk_label_new(user->username);
    gtk_widget_set_halign(label, GTK_ALIGN_START);

    GtkWidget *text_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(text_box), label, FALSE, FALSE, 0);

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    add_class(box, "user");
    char *msg_name = mx_strjoin(USER_NAME, mx_itoa(user->id));
    gtk_widget_set_name(box, msg_name);
    mx_strdel(&msg_name);
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), text_box, FALSE, FALSE, 0);

    GtkWidget *status = gtk_label_new(get_user_privilege_str(user->privilege));
    add_class(status, "count"); 
    gtk_widget_set_halign(status, GTK_ALIGN_END);
    gtk_widget_set_valign(status, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(text_box), status, FALSE, FALSE, 0);

    if (user->id != account->id)
    {
        if (account->current_chat->user_privilege == PRIV_ADMIN)
        {
            GtkWidget *delete_button = create_image_button("Client/icons/trash.png", 20, 20);
            add_class(delete_button, "image");
            gtk_widget_set_halign(delete_button, GTK_ALIGN_END);
            gtk_widget_set_valign(delete_button, GTK_ALIGN_CENTER);
            gtk_box_pack_start(GTK_BOX(box), delete_button, TRUE, TRUE, 0);

            g_signal_connect(delete_button, "clicked", G_CALLBACK(kick_user), (gpointer)user);
        }
    }

    gtk_box_pack_start(GTK_BOX(out_box), box, FALSE, FALSE, 0);

    g_object_unref(pixbuf);
}

void chat_accept_clicked(GtkButton *__attribute__((unused)) button, GtkWidget *window)
{
    GtkWidget *box = get_widget_by_name_r(window, "edit");
    GtkWidget *name_label = get_widget_by_name_r(window, "chatname");

    const gchar *chatname = gtk_entry_get_text(GTK_ENTRY(get_widget_by_name_r(window, "chatname_entry")));

    if (strlen(chatname) && (strcmp(chatname, account->current_chat->name) != 0))
    {
        int error_code = edit_chat_name_in_server(chatname);
        if (error_code != ERR_SUCCESS)
        {
            if (error_code == ERR_CHAT_EXIST)
                pop_up_window("Chat with this name already exists!");
            return;
        }

        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(name_label));

        gtk_text_buffer_set_text(buffer, chatname, -1);
    }

    // else if (!strlen(chatname))
    //     pop_up_window("New name must store at least one character");

    // if ((strcmp(chatname, account->current_chat->name) != 0))
    //     pop_up_window("You didn't change anything");

    clear_box(box);
    gtk_window_resize(GTK_WINDOW(window), 300, 1);
    gtk_widget_show_all(window);

    GtkWidget *widget_to_remove = gtk_grid_get_child_at(GTK_GRID(get_widget_by_name_r(main_window, "chat_grid")), 0, 0);
    gtk_container_remove(GTK_CONTAINER(get_widget_by_name_r(main_window, "chat_grid")), widget_to_remove);
    build_users();

    chat_info();
    chat_menu(NULL);
}
