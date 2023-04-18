#include "../../inc/client.h"

void chat_push_back(t_chat **list, t_chat *new_node)
{
    if (list != NULL && *list == NULL)
    {
        *list = new_node;
        return;
    }

    t_chat *last = *list;
    while (last->next != NULL)
        last = last->next;

    new_node->next = last->next;
    last->next = new_node;
}
