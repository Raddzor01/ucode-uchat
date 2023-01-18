#include "libmx.h"

double mx_pow(double n, unsigned int pow) {
    double mult = 1;
    for (unsigned int i = 0; i < pow; i++) {
        mult *= n;
    }
    return mult;
}
