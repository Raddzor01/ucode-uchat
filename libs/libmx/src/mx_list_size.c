#include "libmx.h"

int tmx_list_size(t_list *list) {
    if (!list)
        return 0;
    t_list *temp = list;
    int size = 0;
    while (temp) {
        temp = temp->next;
        size++;
    }
    return size;
}
