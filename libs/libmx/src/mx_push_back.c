#include "libmx.h"

void mx_push_back(t_list **list, void *data) {
    t_list *temp = mx_create_node(data);
    t_list *some = *list;
    if (*list == NULL) {
        *list = temp;
    } else {
        while (some->next) {
            some = some->next;
        }
        some->next = temp;
    }
}
