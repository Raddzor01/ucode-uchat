#include "../inc/client.h"

void change_chat_id(GtkWidget *widget, int *new_id);

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

void user_box(int number) {
    GtkWidget *button;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *box;
    GtkWidget *out_box = get_widget_by_name_r(main_window, "box_for_users");

    // int *id_link = &account->chat_id_list[number];

    GdkPixbuf *pixbuf;
    GError *error = NULL;

    // printf("\n%s %d\n", account->chat_list[number], account->chat_id_list[number]);

    // Load the image from a file
    pixbuf = gdk_pixbuf_new_from_file("Client/Ass/HOG.png", &error);
    if (error != NULL)
        g_error("Error loading image: %s", error->message);

    // Scale the image to a new size
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);

    // Create a new GtkImage widget from the scaled image
    image = gtk_image_new_from_pixbuf(pixbuf);

    // create a label
    label = gtk_label_new(account->chat_list[number]);

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
    g_signal_connect(button, "clicked", G_CALLBACK(change_chat_id), &account->chat_id_list[number]);
}

void change_chat_id(GtkWidget *widget, int *new_id) {

    (void)widget;

    account->chat_id = *new_id;

    printf("\ncurrent chat: %d\n", account->chat_id);
}

void text_bubble(const char *text) {
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
    gtk_widget_set_hexpand(box, FALSE);

    text_view = gtk_text_view_new();
    // gtk_widget_set_valign(text_view, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(box, FALSE);
    gtk_widget_set_size_request(text_view, -1, -1);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_box_pack_start(GTK_BOX(box), text_view, FALSE, TRUE, 0);

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, text, -1);

    gtk_box_pack_start(GTK_BOX(box_container), box, FALSE, FALSE, 0);

    //redact button
    GtkWidget *edit_button;
    GdkPixbuf *pixbuf;
    GtkWidget *image;

    pixbuf = gdk_pixbuf_new_from_file("Client/icons/pen.png", NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 12, 12, GDK_INTERP_BILINEAR);

    image = gtk_image_new_from_pixbuf(pixbuf);

    edit_button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(edit_button), image);
    gtk_box_pack_start(GTK_BOX(box), edit_button, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(edit_button), "clicked", G_CALLBACK(edit_msg), text_view);

    //delete button
    GtkWidget *delete_button;
    GtkWidget *image2;

    pixbuf = gdk_pixbuf_new_from_file("Client/icons/trash.png", NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 12, 12, GDK_INTERP_BILINEAR);

    image2 = gtk_image_new_from_pixbuf(pixbuf);

    delete_button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(delete_button), image2);
    gtk_box_pack_start(GTK_BOX(box), delete_button, FALSE, FALSE, 0);

    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_msg), box);

    g_object_unref(pixbuf);


    GtkAdjustment *vadjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
    gtk_adjustment_set_value(vadjustment, gtk_adjustment_get_upper(vadjustment) - gtk_adjustment_get_page_size(vadjustment));
    gtk_widget_show_all(box_container);

}

void delete_msg(GtkButton *button, gpointer data){
    GtkWidget *box = (GtkWidget*) data;
    gtk_widget_destroy(box);

    if(button){}
}

void edit_msg(GtkButton *button, gpointer data){

    GtkTextView *text_view = GTK_TEXT_VIEW(data);
    GtkWidget *chat_box = get_widget_by_name_r(main_window, "chat_box");
    GtkWidget *box;
    GtkWidget *entry;
    GtkWidget *edit_button;
    GtkWidget *cancel_button;

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

    entry = gtk_entry_new();
    gtk_widget_set_size_request (entry, 400, -1);
    gtk_entry_set_text(GTK_ENTRY(entry), text);
    gtk_box_pack_start(GTK_BOX(box), entry, TRUE, TRUE, 0);
    info->entry = entry;

    edit_button = gtk_button_new_with_label("edit");
    gtk_widget_set_size_request (edit_button, 100, -1);
    gtk_box_pack_start(GTK_BOX(box), edit_button, FALSE, FALSE, 0);

    g_signal_connect(edit_button, "clicked", G_CALLBACK(edit_accept), text_view);

    //close edit tab
    GdkPixbuf *pixbuf;
    GtkWidget *image;

    pixbuf = gdk_pixbuf_new_from_file("Client/icons/cross.png", NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 14, 14, GDK_INTERP_BILINEAR);

    image = gtk_image_new_from_pixbuf(pixbuf);

    cancel_button = gtk_button_new();
    gtk_button_set_image(GTK_BUTTON(cancel_button), image);
    gtk_box_pack_start(GTK_BOX(box), cancel_button, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(delete_msg), box);

    gtk_widget_show_all(chat_box);
    if(button){}
}

void edit_accept(GtkButton *button, gpointer data) {

    GtkTextView *text_view = GTK_TEXT_VIEW(data);


    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    const gchar *text = gtk_entry_get_text (GTK_ENTRY (info->entry));
    gtk_text_buffer_set_text(buffer, text, strlen(text));
}
