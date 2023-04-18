#include "../../inc/client.h"

void msg_pop_by_message_id(t_msg **list, int msg_id)
{
    t_msg *temp = *list;
    for (int i = 0; temp; temp = temp->next, ++i)
    {
        if (temp->msg_id == msg_id)
        {
            msg_pop_index(list, i);
            return;
        }
    }
}
