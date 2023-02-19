#include "../inc/client.h"
    
// static void create_text_view(GtkWidget *container) {
//     GtkWidget *text_view = gtk_text_view_new();
//     gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
//     gtk_container_add(GTK_CONTAINER(container), text_view);
// }

GtkWidget *main_window;

int main(int argc, char **argv) {

    // server part

    (void)argc;
	srand(time(NULL));

	int server_socket = 0;
	SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;

	connect_to_server(argv[1], atoi(argv[2]), &server_socket, &ctx, &ssl);

    // gtk part

    t_info info;

    info.ssl = ssl;

    // GtkTextBuffer *buffer;

    gtk_init(&argc, &argv);

    log_menu(info);

    gtk_main();

    return 0;
}

