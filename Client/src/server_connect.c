#include "../inc/client.h"

void init_ssl(SSL_CTX **ctx)
{

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    *ctx = SSL_CTX_new(TLS_client_method());
    if (*ctx == NULL)
    {
        exit(EXIT_FAILURE);
    }
}

void connect_ssl(SSL **ssl, int server_fd, SSL_CTX **ctx)
{

    *ssl = SSL_new(*ctx);

    SSL_set_mode(*ssl, SSL_MODE_ASYNC);
    SSL_set_fd(*ssl, server_fd);

    if (SSL_connect(*ssl) == -1)
    {
        mx_printerr(strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void connect_to_server(const char *ip_address, int port, int server_fd,
                       SSL_CTX **ctx, SSL **ssl)
{
    struct sockaddr_in server_addr;

    init_ssl(ctx);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    // inet_aton(ip_address, &server_addr.sin_addr);
    // inet_aton();
    server_addr.sin_port = htons(port);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        mx_printerr("1");
        exit(EXIT_FAILURE);
    }

    if (connect(server_fd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1)
    {
        mx_printerr("2");
        exit(EXIT_FAILURE);
    }

    connect_ssl(ssl, server_fd, ctx);

    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);
    fcntl(server_fd, F_SETFD, O_NONBLOCK);

    info->server_socket = server_fd;
    info->ssl = *ssl;
}
