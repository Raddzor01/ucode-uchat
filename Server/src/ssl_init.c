#include "../inc/server.h"

SSL_CTX *ssl_init() {

    SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();

    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());

    SSL_CTX_use_certificate_file(ctx, SSL_CRT, SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, SSL_KEY, SSL_FILETYPE_PEM);
    SSL_CTX_check_private_key(ctx);

    return ctx;
    
}
