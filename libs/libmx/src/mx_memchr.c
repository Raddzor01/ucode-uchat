#include "libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    unsigned char *src = (unsigned char *)s;
  
    while (n-- > 0) {
        if (*src == c)
            return src;
        src++;
  }
  return NULL;
}
