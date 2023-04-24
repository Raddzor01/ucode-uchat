#include "../../inc/client.h"

void chat_clear_list(t_chat **list)
{
    if (list == NULL || *list == NULL)
        return;

    t_chat *node = *list;
    t_chat *next = NULL;
    while (node)
    {
        next = node->next;
        chat_clear_single_node(&node);
        node = next;
    }

    *list = NULL;
}
