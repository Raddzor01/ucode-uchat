#include "libmx.h"

int mx_quicksort(char **str, int left, int right) {
  if (!str) {
    return -1;
  }
    int i = left;
    int j = right;
    int p;
    int count = 0;
  char *tmp = NULL;
  while (i != j) {
    if ((mx_strlen(str[i]) > mx_strlen(str[j])) == (i < j)) {
      tmp = str[i];
      str[i] = str[j];
      str[j] = tmp;
      count++;

      p = i;
      i = j;

      if (p < j)
        j = p + 1;
      else
        j = p - 1;
    } else {
      if (i < j)
        j--;
      else
        j++;
    }
  }

  if (left < i - 1)
    count += mx_quicksort(str, left, i - 1);
  if (i + 1 < right)
    count += mx_quicksort(str, i + 1, right);

  return count;
}
