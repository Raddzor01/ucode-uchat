#include "libmx.h"

void mx_push_front(t_list **list, void *data) {
    t_list *new_node = mx_create_node(data);
    if (*list == NULL) {
        *list = new_node;  
    }
    else {
        new_node->next = *list;
        *list = new_node;
    }
}
