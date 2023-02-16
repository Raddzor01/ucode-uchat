#include "../inc/client.h"
    
// static void create_text_view(GtkWidget *container) {
//     GtkWidget *text_view = gtk_text_view_new();
//     gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
//     gtk_container_add(GTK_CONTAINER(container), text_view);
// }

SSL *ssl;

int main(int argc, char **argv) {

    (void)argc;
	srand(time(NULL));

	int server_socket = 0;
	SSL_CTX *ctx = NULL;
    ssl = NULL;

	connect_to_server(argv[1], atoi(argv[2]), &server_socket, &ctx, &ssl);

    // normal part

    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *entry;
    GtkWidget *text_view;
    GtkWidget *send_button;
    // GtkTextBuffer *buffer;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chat");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    // gtk_box_pack_start (GTK_BOX (box), text_view, TRUE, TRUE, 0);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);
    g_object_set_data(G_OBJECT(entry), "text_view", text_view);
    g_signal_connect(entry, "activate", G_CALLBACK(send_message), text_view);

    send_button = gtk_button_new_with_label("Send");
    gtk_box_pack_start(GTK_BOX(box), send_button, FALSE, FALSE, 0);
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_button_clicked), entry);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


// int main(int argc, char **argv) {

// 	(void)argc;
// 	srand(time(NULL));

// 	int server_socket = 0;
// 	SSL_CTX *ctx = NULL; 
// 	SSL *ssl = NULL;

// 	connect_to_server(argv[1], atoi(argv[2]), &server_socket, &ctx, &ssl);
// 	char request_str[100] = "";

// 	while(true) {

// 	printf("Write message to server:\n");
// 	scanf("%s", request_str);
//     send_to_server(ssl, request_str);
// 	}

//     return 0;

// }
