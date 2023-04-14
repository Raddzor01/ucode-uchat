#include "../../inc/server.h"

void send_file(cJSON *__attribute__((unused)) json, t_client_info *client_info)
{
    FILE *image = fopen("Server/db/data/image.png", "wb");
    long size;
    if (recv(client_info->socket_info, &size, sizeof(size), 0) < 0)
    {
        mx_logs("recv size 1", ERROR_LOG);
        return;
    }

    char buffer[BUFSIZ];
    size_t bytes_received;
    long bytes_left = size;

    while (bytes_left > 0)
    {
        bytes_received = recv(client_info->socket_info, buffer, BUFSIZ, 0);
        if (bytes_received < 0)
        {
            mx_logs("recv size 2", ERROR_LOG);
            return;
        }
        fwrite(buffer, sizeof(char), bytes_received, image);
        bytes_left -= bytes_received;
    }

    fclose(image);

    int bytes_pending;
    if (ioctl(client_info->socket_info, FIONREAD, &bytes_pending) == -1)
    {
        mx_logs("ioctl", ERROR_LOG);
        return;
    }
    if (bytes_pending > 0)
    {
        fprintf(stderr, "Warning: there are still %d bytes in the receive buffer\n", bytes_pending);
    }
}
