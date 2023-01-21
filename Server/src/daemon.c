#include "../inc/server.h"

void daemon() {

    pid_t pid;
    pid_t sid;
    
    pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(1);
    }

    if (pid > 0) {
        printf("Process id: %d\n", pid);
        exit(0);
    }

    umask(0);
    if ((sid = setsid()) < 0) {
        perror("setsid error");
        exit(1);
    }
    signal(SIGTERM, SIG_DFL);

}
