#include "../inc/server.h"

// Function implements the main logic of processing incoming requests from a client.
void *thread_control(void *arg)
{
    t_client_info *client_info = (t_client_info *)arg;
    char *request = NULL;

    while (true)
    {
        request = read_client_data(client_info->ssl);
        if (!request)
            continue;

        if (handle_request(client_info, request) == REQ_CLIENT_EXIT)
        {
            mx_strdel(&request);
            break;
        }

        mx_strdel(&request);
    }

    pthread_detach(pthread_self());
    return NULL;
}
