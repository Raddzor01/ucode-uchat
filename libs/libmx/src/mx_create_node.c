#include "libmx.h"

t_list *mx_create_node(void *data) {
    t_list *list = (t_list*)malloc(16);
    list->data = data;
    list->next = NULL;
    return list;
}
