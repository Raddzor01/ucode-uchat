#include "../inc/server.h"

void connect_new_client(SSL* ssl, int client_socket) {

    pthread_t thread;

    if (SSL_accept(ssl) == -1) {
        mx_printerr("No SSL accept");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(client_socket,F_GETFL,0);
    fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);
    fcntl(client_socket, F_SETFD, O_NONBLOCK);

    t_client_info* client_info = malloc(sizeof(* client_info));
    client_info->socket_info = client_socket;
    client_info->ssl = ssl;
    client_info->user = NULL;

    pthread_create(&thread, NULL, threads_control, (void *)client_info);

}

// char *read_client_data(SSL *ssl) {
    
//     char buffer[MAX_SEND_DATA];
//     int nbr_of_bytes = SSL_read(ssl, buffer, sizeof(buffer));
//     if(nbr_of_bytes > 0) {
//         return mx_strdup(buffer);
//     } else {
//         return NULL;
//     }
    
// }
char* get_client_request(SSL* ssl, int length) {

    char buffer[MAX_SEND_DATA] = "";
    int bytesRead = 0;
    while (bytesRead < length) {

        int bytes = SSL_read(ssl, &buffer[bytesRead], length - bytesRead );
        if (bytes <= 0) {

            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                continue;
            }
            return NULL;

        } else if (bytes < length) {
            
            bytesRead += bytes;
        
        } else if (bytes == length) {
        
            bytesRead = bytes;
            break;
        
        }
    }
    buffer[bytesRead] = '\0';
    printf("Size %d\nMessage: %s\n", length, buffer);
    return mx_strdup(buffer);

} 

// Read client data (a request to be handled)
char* read_client_data(SSL *ssl) {

    char buffer[MAX_SEND_DATA];
    int bytes = SSL_read(ssl,buffer, MAX_SEND_DATA);
    if(bytes > 0)
        printf("Message from client:\n%s\n", buffer);
    // int n_bytes = 0;
    // while ((n_bytes = SSL_read(ssl, buffer, MAX_SEND_DATA)) <= 0) {
        
    //     if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
    //         continue;
    //     }
    //     return NULL;
    
    // }
    // buffer[n_bytes] = '\0';
    // return mx_strdup(buffer);
    // return get_client_request(ssl, atoi(buffer));
    return 0;

}
