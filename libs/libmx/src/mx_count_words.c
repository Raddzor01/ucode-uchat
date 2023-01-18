#include "libmx.h"

int mx_count_words(const char *str, char delimiter) {
    if (!str) {
        return -1;
    }
    int flag = 0;
    int cnt = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == delimiter)
            flag = 0;
        else if (flag == 0) {
            cnt++;
            flag = 1;
        } 
        if (str[i] == '\0') break;
   }
   return cnt;
}
