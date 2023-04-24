#include "../inc/client.h"

bool init_ssl(SSL_CTX **ctx)
{

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    *ctx = SSL_CTX_new(TLS_client_method());
    if (*ctx == NULL)
        return true;
    return false;
}

bool connect_ssl(SSL **ssl, int server_fd, SSL_CTX **ctx)
{

    *ssl = SSL_new(*ctx);

    SSL_set_mode(*ssl, SSL_MODE_ASYNC);
    SSL_set_fd(*ssl, server_fd);

    if (SSL_connect(*ssl) == -1)
        return true;
    
    return false;
}

bool connect_to_server(const char *ip_address, int port)
{
    int server_fd = 0;
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    struct sockaddr_in server_addr;

    if(init_ssl(&ctx) == true)
        return true;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    // inet_aton(ip_address, &server_addr.sin_addr);
    // inet_aton();
    server_addr.sin_port = htons(port);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return true;

    if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        return true;


    if(connect_ssl(&ssl, server_fd, &ctx))
        return true;

    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);
    fcntl(server_fd, F_SETFD, O_NONBLOCK);

    info->server_socket = server_fd;
    info->ssl = ssl;
    info->ctx = ctx;
    info->port = port;
    info->ip_address = mx_strdup(ip_address);

    return false;
}
