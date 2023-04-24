#include "../../inc/client.h"

void msg_pop_by_message_id(t_msg **head, int id)
{
    t_msg *temp = *head;
    for (int i = 0; temp; temp = temp->next, ++i)
    {
        if (temp->msg_id == id)
        {
            msg_pop_index(head, i);
            return;
        }
    }
}
