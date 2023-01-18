#include "libmx.h"

char *mx_file_to_str(const char *filename) {
    if (!filename) {
        return NULL;
    }
    int size = 0;
    int i = 0;
    char c;
    int file = open(filename, O_RDONLY);
    if (file < 0)
        return NULL;
    while (read(file, &c, 1))
        size++;
    close(file);
    char *str = mx_strnew(size);
    file = open(filename, O_RDONLY);
    if (file < 0) return NULL;
    while (read(file, &c, 1)) {
        str[i] = c;
        i++;
    }
    close(file);
    return str;
}

