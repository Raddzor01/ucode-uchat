#include "../../inc/client.h"

void chat_pop_back(t_chat **head)
{
    if (*head == NULL)
        return;

    if ((*head)->next == NULL)
    {
        mx_strdel(&(*head)->name);
        for (int i = 0; (*head)->users[i] != NULL; i++)
        {
            free((*head)->users[i]);
            (*head)->users[i] = NULL;
        }
        free((*head)->users);
        (*head)->users = NULL;
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
    for (int i = 0; current->users[i] != NULL; i++)
    {
        free(current->users[i]);
        current->users[i] = NULL;
    }
    free(current->users);
    current->users = NULL;
    msg_clear_list(&current->messages);
    free(current);
}
