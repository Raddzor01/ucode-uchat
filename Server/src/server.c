#include "../inc/server.h"

int main(int argc, char *argv[]) {

    if (argc != 2 || argv[1] == NULL) {
        mx_printerr("usage: ./uchat_server [port]\n");
        exit(1);
    }

    struct sockaddr_in server_address;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int server_socket;
    server_init(&server_address, addr_size, &server_socket, argv[1]);

    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    ssl_init(&ctx);

    struct sockaddr_in client_address;
    int client_socket;

    while (true) {

        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_size)) != -1) {
        
        	ssl = SSL_new(ctx);
			SSL_set_fd(ssl, client_socket);
			
			connect_new_client(ssl, client_socket);

        } else {

            mx_logs(strerror(errno), ERROR_LOG);
            
        }
    }
    
    return 1;

}
