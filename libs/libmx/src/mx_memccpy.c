#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
    char t;
     for (size_t i = 0; i < n; i++ ) {
        ((unsigned char*)dst)[i] = t = ((const unsigned char*)src)[i];
        if ( t == '\0' )
            break;
        if (t == c)
            return (void *)&((unsigned char *)dst)[i+1];
     }
    return NULL;
}
