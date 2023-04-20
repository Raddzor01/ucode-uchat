#include "../inc/server.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in client_address;
    socklen_t addr_size;
    int client_socket;
    int server_socket;
    SSL *ssl;
    SSL_CTX *ctx;

    usage_error_check(argc);

    addr_size = sizeof(struct sockaddr_in);
    server_socket = server_init(argv[1]);

    ctx = ssl_ctx_init();

    while (true)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_size);
        if (client_socket == -1)
        {
            mx_logs(strerror(errno), LOG_ERROR);
            continue;
        }

        log_client_conection(client_address.sin_addr);

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_socket);

        connect_new_client(ssl, client_socket);
    }

    return 1;
}
