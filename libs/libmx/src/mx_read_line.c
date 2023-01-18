#include "libmx.h"

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
  if (buf_size > 0 && buf_size < 2147483648) {
    char *buf = mx_strnew(buf_size);
    char *str = mx_strnew(buf_size);
    size_t type;
    if (fd > 0) {
      while ((type = read(fd, buf, buf_size)) > 0) {
        mx_strcat(str, buf);
      }
      int index = mx_get_char_index(str, delim);
      if (index == -1) {
        *lineptr = str;
        return -1;
      } else {
        mx_strncpy(*lineptr, str, index);
      }
      close(fd);
      free(buf);
      free(str);
      return index;
    }
    return -2;
  } else {
    return -2;
  }
}
