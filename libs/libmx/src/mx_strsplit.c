#include "libmx.h"

int CountLetters(const char *s, char c) {
	int i = 0;
	for (;s[i] != c && s[i];i++);
	return i;
}

char **mx_strsplit(const char *s, char c) {
    int count = 0;
    int i = 0;
    if (!s) {
        return NULL;
    }
    char **arr = (char **)malloc((mx_count_words(s, c) + 1) * sizeof(char *));
    while ((*s) && (*s != '\0')) {
        if (*s != c){
            count = CountLetters(s, c);
            arr[i] = mx_strndup(s, count);
            s += count;
            i++;
            continue;
        }
        s++;
    }
    arr[i] = NULL;
    return arr;
}
