#include "../../inc/client.h"

void msg_pop_index(t_msg **list, int id)
{
    int size = 0;
    t_msg *head = *list;
    while (head != NULL)
    {
        head = head->next;
        size++;
    }

    if (id <= 0)
    {
        msg_pop_front(list);
        return;
    }

    else if (id >= size)
    {
        msg_pop_back(list);
        return;
    }

    t_msg *current = *list;
    for (int i = 0; current != NULL && i < (id - 1); ++i)
        current = current->next;

    t_msg *next = current->next->next;
    msg_clear_single_node(&current->next);
    current->next = next;
}
