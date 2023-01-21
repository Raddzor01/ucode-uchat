#include "../inc/server.h"

void server_init(struct sockaddr_in *server_address, socklen_t addr_size, int *server_socket, char *port) {

    daemon();

    (* server_address).sin_addr.s_addr = INADDR_ANY;
    (* server_address).sin_family = AF_INET;
    (* server_address).sin_port = htons(mx_atoi(port));
    
    if((*server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        mx_logs(strerror(errno), ERROR_LOG);
        exit(1);
    }

    if((bind(*server_socket, (struct sockaddr *)server_address, addr_size)) == -1) {
        mx_logs(strerror(errno), ERROR_LOG);
        exit(1);
    }

    if((listen(*server_socket, LISTEN_BACKLOG)) == -1) {
        mx_logs(strerror(errno), ERROR_LOG);
        exit(1);
    }

}


