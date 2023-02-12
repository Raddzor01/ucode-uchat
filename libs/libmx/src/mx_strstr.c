#include "libmx.h"

char *mx_strstr(const char *haystack, const char *needle) {
    char c;
    char sc;
    int len;
    if ((c = *needle++) != 0) {
        len = mx_strlen(needle);
        do {
            do {
                if ((sc = *haystack++) == 0)
                    return (NULL);
            } while (sc != c);
        } while (mx_strncmp(haystack, needle, len) != 0);
        haystack--;
    }
    return ((char *)haystack);
}
