#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    unsigned char *src = (unsigned char *)s + mx_strlen(s);
  
    while (n-- > 0) {
        if (*src == c)
            return src;
        src--;
  }
  return NULL;
}
