#include "../inc/server.h"

int main(int argc, char *argv[]) {

    if (argc != 2 || argv[1] == NULL) {
        mx_printerr("usage: ./uchat_server [port]\n");
        exit(1);
    }
    
    struct sockaddr_in server_address;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(mx_atoi(argv[1]));
    
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_socket, (struct sockaddr *)&server_address, addr_size);
    listen(server_socket, LISTEN_BACKLOG);

    SSL_CTX *ctx = ssl_init();
    SSL *ssl = NULL;

    struct sockaddr_in client_address;
    int client_socket;

    while (true) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_size)) != -1) {
        
        	ssl = SSL_new(ctx);
			SSL_set_fd(ssl, client_socket);
			
			// new_client_create(ssl, client_socket);
        }
    }
    
    return 1;

}
