#include "libmx.h"

static long long get_len(long long number) {
    long long len = 1;

    if (number < 0) {
        len++;
    }

    for (long long i = number; i >= 10 || i <= -10; i /= 10) {
        len++;
    }

    return len;
}

char *mx_ltoa(long long number) {
    long long len = get_len(number);
    char *str = mx_strnew(len);
    long long first = 0;

    if (number < 0) {
        str[0] = '-';
        first = 1;
    }
    for (long long i = len; i > first; i--) {
        long long digit = number % 10;

        if (digit < 0) {
            digit *= -1;
        }

        str[i - 1] = digit + 48;
        number /= 10;
    }

    return str;
}

