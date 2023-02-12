#include "../inc/libmx.h"

void mx_del_2dintarr(int ***matrix, int row) {
    for (int i = 0; i < row; i++) {
        free((* matrix)[i]);
    }
    free(*matrix);
}
