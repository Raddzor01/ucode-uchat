#include "../../inc/client.h"

void chat_move_node_to_head(t_chat **head, int node_id)
{
    if (*head == NULL)
        return;

    if ((*head)->id == node_id)
        return;

    t_chat *prev = NULL;
    t_chat *curr = *head;
    while (curr != NULL && curr->id != node_id) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL)
        return;

    prev->next = curr->next;

    curr->next = *head;
    *head = curr;
}

