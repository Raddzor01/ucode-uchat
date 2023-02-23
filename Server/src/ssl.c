#include "../inc/server.h"

// function initializes the SSL context and returns a pointer to the created context.
SSL_CTX *ssl_ctx_init()
{

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());

    if (!ctx)
    {
        mx_logs("Failed to initialize SSL context", ERROR_LOG);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_certificate_file(ctx, SSL_CRT, SSL_FILETYPE_PEM) <= 0)
    {
        mx_logs("Failed to use SSL certificate file", ERROR_LOG);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, SSL_KEY, SSL_FILETYPE_PEM) <= 0)
    {
        mx_logs("Failed to use SSL private key file", ERROR_LOG);
        exit(EXIT_FAILURE);
    }

    if (!SSL_CTX_check_private_key(ctx))
    {
        mx_logs("Failed to check SSL private key", ERROR_LOG);
        exit(EXIT_FAILURE);
    }

    return ctx;
}
