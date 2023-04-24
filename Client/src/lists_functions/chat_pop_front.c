#include "../../inc/client.h"

void chat_pop_front(t_chat **head)
{
    if (*head == NULL)
        return;

    t_chat *temp = *head;
    *head = (*head)->next;
    mx_strdel(&temp->name);
    msg_clear_list(&temp->messages);
    free(temp);
}
