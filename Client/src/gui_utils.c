#include "../inc/client.h"

void load_css () {
	GtkCssProvider *styles = gtk_css_provider_new();
    gtk_css_provider_load_from_path(styles, "Client/src/style/main.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(styles), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void add_class(GtkWidget *widget, char *class_name) {
	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	gtk_style_context_add_class(context, class_name);
}

void clear_window(GtkWidget *window)
{
    // Get the container widget of the window
    GtkWidget *container = gtk_bin_get_child(GTK_BIN(window));

    // Remove any existing widget from the container
    if (GTK_IS_WIDGET(container))
    {
        gtk_container_remove(GTK_CONTAINER(window), container);
    }
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
        else if(GTK_IS_CONTAINER(children->data))
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

void user_box(GtkWidget *widget) {
    GtkWidget *button;
    GtkWidget *image;
    GtkWidget *label;
    GtkWidget *box;

    GdkPixbuf *pixbuf;
    GError *error = NULL;

    // Load the image from a file
    pixbuf = gdk_pixbuf_new_from_file("Client/Ass/HOG.png", &error);
    if (error != NULL) {
        g_error("Error loading image: %s", error->message);
    }

    // Scale the image to a new size
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 50, 50, GDK_INTERP_BILINEAR);

    // Create a new GtkImage widget from the scaled image
    image = gtk_image_new_from_pixbuf(pixbuf);

    // create a label
    label = gtk_label_new("User Hog");

    // create a box to hold the image and label
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // create a button
    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), box);
    gtk_box_pack_start(GTK_BOX(widget), button, FALSE, FALSE, 0);
}
