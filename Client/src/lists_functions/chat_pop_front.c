#include "../../inc/client.h"

void chat_pop_front(t_chat **head)
{
    if (*head == NULL)
        return;

    t_chat *temp = *head;
    *head = (*head)->next;
    mx_strdel(&temp->name);
    for (int i = 0; temp->users[i] != NULL; i++)
    {
        free(temp->users[i]);
        temp->users[i] = NULL;
    }
    free(temp->users);
    temp->users = NULL;
    
    msg_clear_list(&temp->messages);
    free(temp);
}
