#include "../../inc/client.h"

void msg_push_back(t_msg **list, t_msg *new_node)
{
    if (list != NULL && *list == NULL)
    {
        *list = new_node;
        return;
    }

    t_msg *last = *list;
    while (last->next != NULL)
        last = last->next;

    new_node->next = last->next;
    last->next = new_node;
}
