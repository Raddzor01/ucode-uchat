#include "../inc/server.h"

void *threads_control(void *arg) {

    t_client_info *client_info = (t_client_info *)arg;

    while (true) {

        char *request = NULL;

        if(!(request = read_client_data(client_info->ssl))) {
            continue;
        }

        mx_strdel(&request);
    }
    
    pthread_detach(pthread_self());
    return NULL;
}

