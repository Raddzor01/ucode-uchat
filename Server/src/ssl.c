#include "../inc/server.h"

void ssl_init(SSL_CTX **ctx) {

    SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();

    *ctx = SSL_CTX_new(TLS_server_method());

	if (!*ctx) {
		mx_logs("SSL CTX init error", ERROR_LOG);
		exit(EXIT_FAILURE);
	}

	if (SSL_CTX_use_certificate_file(*ctx, SSL_CRT, SSL_FILETYPE_PEM) <= 0)
	{
		mx_logs("SSL CTX use certificate error", ERROR_LOG);
		exit(EXIT_FAILURE);
	}

	if (SSL_CTX_use_PrivateKey_file(*ctx, SSL_KEY, SSL_FILETYPE_PEM) <= 0)
	{
		mx_logs("SSL CTX use key error", ERROR_LOG);
		exit(EXIT_FAILURE);
	}

	if (!SSL_CTX_check_private_key(*ctx))
	{
		mx_logs("SSL CTX check key error", ERROR_LOG);
		exit(EXIT_FAILURE);
	}


}
