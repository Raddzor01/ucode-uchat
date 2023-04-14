#include "../inc/client.h"

// static void create_text_view(GtkWidget *container) {
//     GtkWidget *text_view = gtk_text_view_new();
//     gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
//     gtk_container_add(GTK_CONTAINER(container), text_view);
// }
void freeing_memory();
void set_start_of_current_day();

GtkWidget *main_window;
t_info *info;
t_account *account;


int main(int argc, char **argv) {

  // announcement part

  (void)argc;
  srand(time(NULL));

  info = malloc(sizeof(*info));
  account = malloc(sizeof(*account));
  account->chat_id = -1;

  set_start_of_current_day();

  // server part

  int server_socket = 0;
  SSL_CTX *ctx = NULL;
  SSL *ssl = NULL;

  connect_to_server(argv[1], atoi(argv[2]), &server_socket, &ctx, &ssl);

  info->ssl = ssl;

  // gtk part

  gtk_init(&argc, &argv);

  load_css();

  log_menu(main_window);

  gtk_main();

  send_exit_to_server();

  freeing_memory();

  return 0;
}

void freeing_memory() {
  for (int i = 0; i < account->chat_count; i++)
    free(account->chat_list[i]);

  free(account->username);
  free(account->chat_id_list);

  free(account);

  free(info->ssl);

  free(info);
}

void set_start_of_current_day() {

  struct tm *u;
  const time_t timer = time(NULL);

  u = localtime(&timer);

  u->tm_sec = 0;
  u->tm_min = 0;
  u->tm_hour = 0;

  info->start_of_current_day = timer;
  // char *f = settime(u);
  // printf("%ld соответствует %s", timer, f);

  // free(f);
  // free(u);
}
