#include "../../inc/client.h"

void msg_pop_front(t_msg **head)
{
    if (head == NULL || *head == NULL)
        return;

    if ((*head)->next == NULL)
    {
        msg_clear_single_node(head);
        *head = NULL;
        return;
    }

    t_msg *temp = *head;
    *head = (*head)->next;
    msg_clear_single_node(&temp);
}
