#include "../inc/client.h"

guint cancel_handler_id;
guint accept_handler_id;
guint cancel_entry_id;
guint accept_entry_id;
bool username_display;


void change_chat_id(GtkWidget *widget, gpointer new_id);

char *settime(struct tm *u) {

  char s[40];
  char *tmp;

  for (int i = 0; i<40; i++) 
    s[i] = 0;

  int length = strftime(s, 40, "%d.%m.%Y %H:%M:%S, %A", u);

  if (length == 0)
    return NULL;

  tmp = (char*)malloc(sizeof(s));

  mx_strcpy(tmp, s);

  return(tmp);
}

void load_css () {
	GtkCssProvider *styles = gtk_css_provider_new();
    gtk_css_provider_load_from_path(styles, "Client/src/style/main.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(styles), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void add_class(GtkWidget *widget, char *class_name) {
	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	gtk_style_context_add_class(context, class_name);
}

void clear_window(GtkWidget *window) {
    // Get the container widget of the window
    GtkWidget *container = gtk_bin_get_child(GTK_BIN(window));

    // Remove any existing widget from the container
    if (GTK_IS_WIDGET(container)) 
        gtk_container_remove(GTK_CONTAINER(window), container);
}

void clear_box(GtkWidget *box) {

    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(box));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_container_remove(GTK_CONTAINER(box), GTK_WIDGET(iter->data));
    }
    g_list_free(children);
}

void create_new_window(char *title, int width, int height, bool resizable) 
{
    if (!main_window) {
        main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 0);
        g_signal_connect(main_window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);
    }

    gtk_window_set_title(GTK_WINDOW(main_window), title);
    gtk_window_resize(GTK_WINDOW(main_window), width, height);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(main_window), resizable);
}

GtkWidget* create_image_button(char* image_path, int width, int height) {
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

GtkWidget *get_widget_by_name_r(GtkWidget *container, char *name) {
    GtkWidget *result = NULL; 
    GList *children = NULL;

    if (GTK_IS_CONTAINER(container)) {
        children = gtk_container_get_children(GTK_CONTAINER(container));
    } else {
        return NULL;
    }

    while (children) {
        if (!mx_strcmp(gtk_widget_get_name(GTK_WIDGET(children->data)), name)) {
            result = GTK_WIDGET(children->data);
            break;
        } else if (GTK_IS_CONTAINER(children->data)) {
            result = get_widget_by_name_r(children->data, name);

            if (result != NULL)
                break;
        }

        children = children->next;   
    }

    g_list_free(g_steal_pointer(&children)); // g_list_free(children_c); //

    return result;
}

void file_select(GtkWidget *widget, gpointer data) {

    if(data){};
        (void)widget;

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File", NULL, action, "_Cancel", 
                                         GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        // send_file_to_server(filename);
        
        // Do something with the selected file...
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void user_box(char *username, int id) {
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
    label = gtk_label_new(username);

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
    g_signal_connect(button, "clicked", G_CALLBACK(change_chat_id), GINT_TO_POINTER(id));
}

void change_chat_id(GtkWidget *widget, gpointer new_id) {

    (void)widget;

    account->chat_id = GPOINTER_TO_INT(new_id);

    printf("\ncurrent chat: %d\n", account->chat_id);

    GtkWidget *chat = get_widget_by_name_r(main_window, "box_holder");
    clear_box(chat);

    t_msg **msg = get_chat_messages_from_server(account->chat_id);

    // printf("\ncurrent chat: %d\n", account->chat_id);

    for (int i = 0; msg[i] != NULL; i++) {
        if (msg[i]->date > info->start_of_current_day) {
            // today
            if (msg[i]->user_id == account->id) {
                text_bubble(msg[i]->text, msg[i]->msg_id);
                printf("%s\n", settime(localtime(&msg[i]->date)));
                free(msg[i]->text);
            } else {
            // receive_bubble()
            }
        } else {
            // not today
            if (msg[i]->user_id == account->id) {
                text_bubble(msg[i]->text, msg[i]->msg_id);
                printf("%s\n", settime(localtime(&msg[i]->date)));
                free(msg[i]->text);
            } else {
            // receive_bubble()
            }
        }
    }
    
    free(msg);
}

void receive_bubble(const char *text, const char *name) {
    GtkWidget *box_container = get_widget_by_name_r(main_window, "box_holder");
    GtkWidget *scrolled_window = get_widget_by_name_r(main_window, "scroll");

    GtkWidget *username_box;
    GtkWidget *username;
    GtkWidget *text_view;
    GtkWidget *box;
    GtkTextBuffer *buffer;

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_START);

    //username
    if (username_display) {
        GdkPixbuf *pixbuf;
        GtkWidget *image;

        pixbuf = gdk_pixbuf_new_from_file("Client/Ass/HOG.png", NULL);
        pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);
        image = gtk_image_new_from_pixbuf(pixbuf);
        g_object_unref(pixbuf);

        gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    } else {
        GtkWidget *blank_space = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_set_size_request(blank_space, 50, -1);
        gtk_box_pack_start(GTK_BOX(box), blank_space, FALSE, FALSE, 0);
    }

    //text bubble
    username_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    add_class(username_box, "receive");
    if (username_display) {
        username = gtk_label_new(name);
        gtk_widget_set_halign(username, GTK_ALIGN_START);
        gtk_widget_set_valign(username, GTK_ALIGN_END);
        gtk_box_pack_start(GTK_BOX(username_box), username, FALSE, FALSE, 0);
    }
    gtk_widget_set_halign(username_box, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), username_box, FALSE, FALSE, 0);

    //text
    text_view = gtk_text_view_new();

    gtk_widget_set_valign(text_view, GTK_ALIGN_END);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_box_pack_start(GTK_BOX(username_box), text_view, FALSE, TRUE, 0);

    PangoLayout *layout = gtk_widget_create_pango_layout(text_view, text);
    int width, height;
    pango_layout_get_pixel_size(layout, &width, &height);

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_NONE);

    if (width > 400) {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
        gtk_widget_set_size_request(text_view, 400, -1);
        gtk_widget_set_hexpand(text_view, TRUE);
        gtk_widget_set_halign(text_view, GTK_ALIGN_FILL);
        gtk_widget_set_hexpand(username_box, TRUE);
        gtk_widget_set_halign(username_box, GTK_ALIGN_START);
    }

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, text, strlen(text));

    GtkAdjustment *vadjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
    gtk_adjustment_set_value(vadjustment, gtk_adjustment_get_upper(vadjustment) - gtk_adjustment_get_page_size(vadjustment));
    
    gtk_widget_show_all(box_container);
    gtk_widget_queue_draw(main_window);
    username_display = FALSE;
}

void text_bubble(const char *text, int msg_id) {
    username_display = TRUE;
    GtkWidget *box_container = get_widget_by_name_r(main_window, "box_holder");
    GtkWidget *scrolled_window = get_widget_by_name_r(main_window, "scroll");
    GtkWidget *box;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;

    //text part
    gtk_widget_set_hexpand(box_container, FALSE);

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_halign(box, GTK_ALIGN_END);
    gtk_widget_set_size_request(box, -1, 30);
    add_class(box, "bubble");
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_vexpand(box,FALSE);

    gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);

    text_view = gtk_text_view_new();

    gtk_widget_set_valign(text_view, GTK_ALIGN_CENTER);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    // gtk_box_pack_start(GTK_BOX(box), text_view, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(box), text_view);

    PangoLayout *layout = gtk_widget_create_pango_layout(text_view, text);
    int width, height;
    pango_layout_get_pixel_size(layout, &width, &height);

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_NONE);

    if (width > 400) {
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
        gtk_widget_set_size_request(text_view, 400, -1);
        gtk_widget_set_hexpand(text_view, TRUE);
        gtk_widget_set_halign(text_view, GTK_ALIGN_END);

        gtk_widget_set_hexpand(box, TRUE);
        gtk_widget_set_halign(box, GTK_ALIGN_END);
    }

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, text, strlen(text));

    // gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);

    //redact button
    GtkWidget *edit_button;

    edit_button = create_image_button("Client/icons/pen.png", 12, 12);
    gtk_box_pack_start(GTK_BOX(box), edit_button, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(change_msg_id_for_edit), GINT_TO_POINTER(msg_id));
    g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(edit_msg), text_view);

    //delete button
    GtkWidget *delete_button;

    delete_button = create_image_button("Client/icons/trash.png", 12, 12);
    gtk_box_pack_start(GTK_BOX(box), delete_button, FALSE, FALSE, 0);

    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_msg_id), GINT_TO_POINTER(msg_id));
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_msg), box);

    GtkAdjustment *vadjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
    gtk_adjustment_set_value(vadjustment, gtk_adjustment_get_upper(vadjustment) - gtk_adjustment_get_page_size(vadjustment));
    
    gtk_widget_show_all(box_container);
    gtk_widget_queue_draw(main_window);
}

void delete_msg(GtkButton *button, gpointer data) {

    GtkWidget *box = (GtkWidget*) data;
    gtk_widget_destroy(box);

    if(button){}
}

void delete_msg_id(GtkButton *button, gpointer msg_id) {

    delete_msg_in_server(GPOINTER_TO_INT(msg_id));

    if(button){}
}

void cancel_edit(GtkButton *button, gpointer data) {

    GtkWidget *send_button = get_widget_by_name_r(main_window, "send_button");
    GtkWidget *entry = get_widget_by_name_r(main_window, "chat_text_entry");

    g_signal_handler_disconnect(G_OBJECT(send_button), cancel_handler_id);
    g_signal_handler_disconnect(G_OBJECT(send_button), accept_handler_id);
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), NULL);

    g_signal_handler_disconnect(G_OBJECT(entry), cancel_entry_id);
    g_signal_handler_disconnect(G_OBJECT(entry), accept_entry_id);
    g_signal_connect(entry, "activate", G_CALLBACK(send_message), NULL);

    gtk_entry_set_text (GTK_ENTRY (info->entry), "");

    GtkWidget *box = (GtkWidget*) data;
    gtk_widget_destroy(box);

    if(button){}
}

void edit_msg(GtkButton *button, gpointer data) {

    GtkTextView *text_view = GTK_TEXT_VIEW(data);
    GtkWidget *chat_box = get_widget_by_name_r(main_window, "chat_box");
    GtkWidget *box;
    GtkWidget *cancel_button;
    GtkWidget *edit_text;
    GtkWidget *entry = get_widget_by_name_r(main_window, "chat_text_entry");
    GtkWidget *send_button = get_widget_by_name_r(main_window, "send_button");

    //take text from bubble (to set it to entry)
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    //create redact field
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_set_size_request(box, -1, -1);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_FILL);
    gtk_box_pack_start(GTK_BOX(chat_box), box, FALSE, FALSE, 0);

    //edited text
    edit_text = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(edit_text), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(edit_text), FALSE);
    gtk_widget_set_size_request(edit_text, -1, -1);
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
    if(button){}
}

void edit_accept(GtkButton *button, gpointer data) {

    GtkTextView *text_view = GTK_TEXT_VIEW(data);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    const gchar *text = gtk_entry_get_text (GTK_ENTRY (info->entry));
    if (strlen(text) == 0)
        return;
        
    gtk_text_buffer_set_text(buffer, text, strlen(text));
    edit_msg_in_server(info->msg_id_for_edit, text);

    if(button){}
}

void change_msg_id_for_edit(GtkButton *button, gpointer msg_id) {

    info->msg_id_for_edit = GPOINTER_TO_INT(msg_id);

    if(button){}
}

void create_chat(GtkButton *button, gpointer chatname) {
    (void)button;
    const char *text = gtk_entry_get_text(GTK_ENTRY(GTK_ENTRY(chatname)));

    create_chat_in_server(text, CHAT_NORMAL);

    // int chat_id = check_chat_id_from_server();
}
