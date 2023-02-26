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
        gtk_window_set_title(GTK_WINDOW(main_window), title);
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

