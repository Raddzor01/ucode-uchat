#include "../../inc/client.h"

char *read_from_server()
{
    char buffer[BUFSIZ];
    int bytes = 0;
    while ((bytes = SSL_read(info->ssl, buffer, sizeof(buffer)) <= 0))
    {
        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
            continue;
        return NULL;
    }
    return mx_strdup(buffer);
}

void read_from_server_to_logs()
{
    char *json_str = read_from_server();

    mx_logs(json_str, LOG_INFO);

    free(json_str);
}
