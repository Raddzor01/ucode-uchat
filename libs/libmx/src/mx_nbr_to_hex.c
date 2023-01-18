#include "libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
    char *res = mx_strnew(100);
    int temp = 0;
    int i = 0;
    int j = 0;
    char temp_char;
    while (nbr != 0) {
        temp = nbr % 16;
        if (temp < 10)
            temp += 48;
        else
            temp += 87;
        res[i] = temp;
        i++;
        nbr /= 16;
    }
    while (res[j] != '\0')
        j++;
    j--;
    for (i = 0; i < j; i++) {
        temp_char = res[i];
        res[i] = res[j];
        res[j] = temp_char;
        j--;
    }
    return res;
}
