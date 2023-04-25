#include "../inc/client.h"

void *server_online_check_thread()
{
    bool is_connected = false;
    while (true)
    {
        if (check_server_online() == true)
        {
            mx_printerr("Lost connection to the server! Trying to reconnect...");
#ifdef MACOS_VER
            sem_wait(account->semaphore);
#else
            sem_wait(&account->semaphore);
#endif
            close(info->server_socket);
            SSL_free(info->ssl);
            SSL_CTX_free(info->ctx);
            while (true)
            {
                is_connected = connect_to_server(info->ip_address, info->port);
                if (is_connected == true)
                    g_usleep(200000);
                else if (is_connected == false)
                    break;
            }
            send_login_to_server(account->username, account->password);
#ifdef MACOS_VER
            sem_post(account->semaphore);
#else
            sem_post(&account->semaphore);
#endif
            mx_printerr("Reconnection complete\n");
        }
        g_usleep(100000);
    }
}
