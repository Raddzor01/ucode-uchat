#include "../inc/libmx.h"

void mx_logs(char* info, bool is_error) {
    FILE* fd = fopen("Server/info/server_logs.log", "a+");
    fprintf(fd, "%s: %s\n", is_error == 1 ? "ERROR" : "INFO", info);
    fclose(fd);
}
