#include "libmx.h"

char *mx_strndup(const char *str, size_t n) {

    size_t length = mx_strlen(str);

    if (n < length) {
        length = n;
    }

    char *duplicate = mx_strnew(length);
    mx_strncpy(duplicate, str, length);
    
    return duplicate;
}
