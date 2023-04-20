#include "../inc/server.h"

// initialize the server with the specified port number
int server_init(char *port)
{
    struct sockaddr_in server_address;
    int server_socket;

    daemon();

    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(mx_atoi(port));

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        mx_printerr(strerror(errno));
        mx_printerr("\n");
        mx_logs(strerror(errno), LOG_ERROR);
        exit(EXIT_FAILURE);
    }

    if ((bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address))) == -1)
    {
        mx_printerr(strerror(errno));
        mx_printerr("\n");
        mx_logs(strerror(errno), LOG_ERROR);
        exit(EXIT_FAILURE);
    }

    if ((listen(server_socket, LISTEN_BACKLOG)) == -1)
    {
        mx_printerr(strerror(errno));
        mx_printerr("\n");
        mx_logs(strerror(errno), LOG_ERROR);
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

// make current process a daemon
void daemon()
{
    pid_t sid;
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        mx_printerr("Failed to fork\n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        printf("Process id: %d \n", pid);
        exit(EXIT_SUCCESS);
    }

    umask(0);
    sid = setsid();

    if (sid < 0)
    {
        mx_printerr("Failed to set session id\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGTERM, SIG_DFL);
}

// usage check
void usage_error_check(int argc)
{

    if (argc != 2)
    {
        mx_printerr("usage: ./uchat_server [port]\n");
        exit(EXIT_FAILURE);
    }
}

// logging client connection
void log_client_conection(struct in_addr sa)
{
    char client_ip[INET_ADDRSTRLEN];
    char *log_message;

    inet_ntop(AF_INET, &sa, client_ip, INET_ADDRSTRLEN);

    log_message = mx_strjoin("Trying to connect client: ", client_ip);
    mx_logs(log_message, LOG_INFO);

    mx_strdel(&log_message);
}
