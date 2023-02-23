#include "../inc/server.h"

//  Function connects a new client to the server by accepting the SSL connection and sets the client's socket to non-blocking mode. It then creates a new thread for the client using the thread_control function, passing the client information including socket number and SSL information as an argument.
void connect_new_client(SSL *ssl, int client_socket)
{

    pthread_t thread;

    if (SSL_accept(ssl) == -1)
    {
        mx_logs(strerror(errno), ERROR_LOG);
        exit(EXIT_FAILURE);
    }

    // Set the client socket to non-blocking mode
    int flags = fcntl(client_socket, F_GETFL, 0);
    fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

    t_client_info *client_info = (t_client_info *)malloc(sizeof(*client_info));
    client_info->socket_info = client_socket;
    client_info->ssl = ssl;
    client_info->user = NULL;

    pthread_create(&thread, NULL, thread_control, client_info);
}

// read the information received from the client and write it as a string
char *read_client_data(SSL *ssl)
{

    char buffer[MAX_BUF_SIZE];
    int n_bytes = SSL_read(ssl, &buffer, sizeof(buffer));

    if (n_bytes > 0)
    {
        buffer[n_bytes] = 0;
        return mx_strdup(buffer);
    }

    return NULL;
}
