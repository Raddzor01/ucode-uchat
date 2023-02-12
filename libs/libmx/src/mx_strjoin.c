#include "libmx.h"

char *mx_strjoin(char const *s1, char const *s2) {
    if (s1 == NULL && s2 != NULL) {
        return mx_strdup(s2);
    }
    if (s1 != NULL && s2 == NULL) {
        return mx_strdup(s1);
    }
    if (s1 == NULL && s2 == NULL) {
        return NULL;
    }
    int len = mx_strlen(s1) + mx_strlen(s2);
    char *join = mx_strnew(len);
    join = mx_strcat(join, s1);
    join = mx_strcat(join, s2);
    return join;
}

