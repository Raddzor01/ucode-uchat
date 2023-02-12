#include "libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    if (!str || !sub) {
        return -2;
    }
    int len = mx_strlen(sub);
    char *check = mx_strnew(len);

    for (int i = 0; str[i]; i++) {
        if (str[i] == sub[0]) {
            check = mx_strncpy(check, &str[i], len);

            if (mx_strcmp(check, sub) == 0) {
                return i;
            }
        }
    }
    return -1;
}
