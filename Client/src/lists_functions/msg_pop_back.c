#include "../../inc/client.h"

void msg_pop_back(t_msg **head)
{
    if (head == NULL || *head == NULL)
        return;

    if ((*head)->next == NULL)
    {
        msg_clear_single_node(head);
        return;
    }

    t_msg *prelast = *head;
    while (prelast->next->next != NULL)
        prelast = prelast->next;
        
    msg_clear_single_node(&prelast->next);
    prelast->next = NULL;
}
