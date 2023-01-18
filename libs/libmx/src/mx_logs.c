#include "../inc/libmx.h"

void mx_logs(const char* info, bool is_error) {

    FILE* fd = fopen("../../../info/logs.log", "a+");
    fprintf(fd, "%s: ", is_error == 1 ? "ERROR" : "INFO");
    fprintf(fd, "%s\n", info);
    fclose(fd);

}
