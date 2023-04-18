#include "../../inc/client.h"

void msg_clear_list(t_msg **list)
{
    if (list == NULL || *list == NULL)
        return;

    t_msg *node = *list;
    t_msg *next = NULL;
    while (node != NULL)
    {
        next = node->next;
        msg_clear_single_node(&node);
        node = next;
    }

    *list = NULL;
}

