#include "../../inc/client.h"

void chat_pop_back(t_chat **head)
{
    if (*head == NULL)
        return;

    if ((*head)->next == NULL)
    {
        mx_strdel(&(*head)->name);
        msg_clear_list(&(*head)->messages);
        free(*head);
        *head = NULL;
        return;
    }

    t_chat *prev = *head;
    t_chat *current = (*head)->next;
    while (current->next != NULL)
    {
        prev = current;
        current = current->next;
    }

    prev->next = NULL;
    mx_strdel(&current->name);
    msg_clear_list(&current->messages);
    free(current);
}
