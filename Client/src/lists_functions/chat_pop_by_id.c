#include "../../inc/client.h"

void chat_pop_by_id(t_chat **head, int id)
{
    t_chat *temp = *head;
    for (int i = 0; temp; temp = temp->next, ++i)
    {
        if (temp->id == id)
        {
            chat_pop_index(head, i);
            return;
        }
    }
}
