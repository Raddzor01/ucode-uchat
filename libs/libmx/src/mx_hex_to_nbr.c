#include "libmx.h"

unsigned long mx_hex_to_nbr(const char *hex) {
    if (hex == NULL) return 0;
    int size = 0;
    unsigned long long int res = 0;
    while (hex[size] != '\0')
        size++;
    size--;
    unsigned long int hex_count = 1;
    for (int i = size; i >= 0; i--) {
        if ((hex[i] >= 65
        && hex[i] <= 90)
        || (hex[i] >= 95 
        && hex[i] <= 122)) {
            if (hex[i] >= 97
            && hex[i] <= 122)
                res += (hex[i] - 87) * hex_count;
            if (hex[i] >= 65
            && hex[i] <= 90)
                res += (hex[i] - 55) * hex_count;
        }
        if (hex[i] >= 48
        && hex[i] <= 57)
            res += (hex[i] - 48) * hex_count;
        hex_count *= 16;
    }
    return res;
}
