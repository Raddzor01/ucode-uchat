#include "../inc/server.h"

void connect_new_client(SSL* ssl, int client_socket) {

    pthread_t thread;

    if (SSL_accept(ssl) == -1) {
        mx_logs("No SSL accept from client", ERROR_LOG);
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(client_socket, F_GETFL, 0);
    fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
    fcntl(client_socket, F_SETFD, O_NONBLOCK);

    t_client_info* client_info = malloc(sizeof(* client_info));
    client_info->socket_info = client_socket;
    client_info->ssl = ssl;
    client_info->user = NULL;

    pthread_create(&thread, NULL, threads_control, (void *)client_info);

}

char* read_client_data(SSL *ssl) {

    char buffer[MAX_SEND_DATA];
    int n_bytes = SSL_read(ssl, &buffer, sizeof(buffer));

    if(n_bytes > 0) {
        buffer[n_bytes] = 0;
        return mx_strdup(buffer);
    }
    
    return NULL;

}
