#include "libmx.h"

char *mx_strdup(const char *str) {
    int size = mx_strlen(str);
    char *res = mx_strnew(size);
    
    res = mx_strcpy(res, str);

    return res;
}
