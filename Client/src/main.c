#include "../inc/client.h"

// static void create_text_view(GtkWidget *container) {
//     GtkWidget *text_view = gtk_text_view_new();
//     gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
//     gtk_container_add(GTK_CONTAINER(container), text_view);
// }

GtkWidget *main_window;
t_info *info;
t_account *account;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void free_memory();
static void set_start_of_current_day();

static void structs_init()
{
    info = malloc(sizeof(*info));
    account = malloc(sizeof(*account));

    pthread_mutex_init(&account->mutex, NULL);
    account->chats = NULL;
    account->username = NULL;
    account->current_chat = NULL;
    account->is_busy = false;
    set_start_of_current_day();

    info->ctx = NULL;
    info->ssl = NULL;
    info->server_socket = 0;
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    structs_init();

    // server part
    connect_to_server(argv[1], atoi(argv[2]));

    // gtk part
    gtk_init(&argc, &argv);

    load_css();

    log_menu(main_window);

    pthread_create(&account->server_update_thread, NULL, server_update_thread, NULL);

    gtk_main();

    send_exit_to_server();

    free_memory();

    return 0;
}

void set_start_of_current_day()
{

    struct tm *u;
    const time_t timer = time(NULL);

    u = localtime(&timer);

    u->tm_sec = 0;
    u->tm_min = 0;
    u->tm_hour = 0;

    info->current_day_time = mktime(u);
    // char *f = settime(u);
    // printf("%ld соответствует %s", timer, f);

    // free(f);
    // free(u);
}

void free_memory()
{
    pthread_mutex_destroy(&account->mutex);

    SSL_free(info->ssl);
    SSL_CTX_free(info->ctx);
    free(info);
    info = NULL;

    free(account);
    account = NULL;
}

