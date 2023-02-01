#include "../inc/server.h"

int main(int argc, char *argv[]) {

    usage_error_check(argc);

    socklen_t addr_size = sizeof(struct sockaddr_in);
    int server_socket = server_init(argv[1]);

    SSL_CTX *ctx = ssl_ctx_init();

    struct sockaddr_in client_address;
    int client_socket;

    while (true) {

        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addr_size);
        if(client_socket == -1) {
            mx_logs(strerror(errno), ERROR_LOG);
            continue;
        }
        
        log_client_conection(client_address.sin_addr);
        
        SSL *ssl = SSL_new(ctx);
		SSL_set_fd(ssl, client_socket);
			
		connect_new_client(ssl, client_socket);
            
    }
    
    return 1;

}
