#include "../../inc/client.h"

void chat_pop_index(t_chat **list, int id)
{
    int size = 0;
    t_chat *head = *list;
    while (head != NULL)
    {
        head = head->next;
        size++;
    }

    if (id <= 0)
    {
        chat_pop_front(list);
        return;
    }

    else if (id >= size)
    {
        chat_pop_back(list);
        return;
    }

    t_chat *current = *list;
    for (int i = 0; current != NULL && i < (id - 1); ++i)
        current = current->next;

    t_chat *next = current->next->next;
    chat_clear_single_node(&current->next);
    current->next = next;
}
