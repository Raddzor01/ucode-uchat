#include "../inc/client.h"

int send_to_server(SSL *ssl, const char* request_str) {

    int req_len = mx_strlen(request_str);
	SSL_write(ssl, request_str, req_len);
    return 0;

}

void init_ssl(SSL_CTX **ctx) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    
    *ctx = SSL_CTX_new(TLS_client_method());
    if (*ctx == NULL) {
		exit(EXIT_FAILURE);
    } 
}

void connect_ssl(SSL **ssl, int* server_fd, SSL_CTX **ctx) {
	*ssl = SSL_new(*ctx);
	SSL_set_mode(*ssl, SSL_MODE_ASYNC);
	SSL_set_fd(*ssl, *server_fd);

	if (SSL_connect(*ssl) == -1) {
        mx_printerr("3");
		exit(EXIT_FAILURE);
	}
}

void connect_to_server(const char* ip_address, int port, int* server_fd, SSL_CTX **ctx, SSL **ssl) {

    struct sockaddr_in server_addr;

	init_ssl(ctx);

    server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip_address);
	// inet_aton(ip_address, &server_addr.sin_addr);
	// inet_aton();
	server_addr.sin_port = htons(port);
	
	if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {		
		mx_printerr("1");
		exit(EXIT_FAILURE);
	}

	if (connect(*server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		mx_printerr("2");
		exit(EXIT_FAILURE);
	}

	connect_ssl(ssl, server_fd, ctx);
}


void send_message(GtkWidget *entry, GtkTextView *text_view) {
    GtkTextBuffer *buffer;
    GtkTextIter iter;
    const gchar *text;

    buffer = gtk_text_view_get_buffer (text_view);
    text = gtk_entry_get_text (GTK_ENTRY (entry));
    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_buffer_insert (buffer, &iter, text, -1);
    gtk_text_buffer_insert (buffer, &iter, "\n", -1);
    gtk_entry_set_text (GTK_ENTRY (entry), "");
    gtk_text_view_scroll_to_iter (text_view, &iter, 0.0, FALSE, 0.0, 0.0);
}

static void send_button_clicked(GtkButton *button, GtkWidget *entry) {
    GtkTextView *text_view;

    text_view = g_object_get_data (G_OBJECT (entry), "text_view");
    send_message (entry, text_view);
}
    
static void create_text_view(GtkWidget *container) {
    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
    gtk_container_add(GTK_CONTAINER(container), text_view);
}

int main(int argc, char **argv) {
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
