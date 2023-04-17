#include "../inc/client.h"

void msg_push_back(t_msg **list, t_msg *new_node)
{
    if (list != NULL && *list == NULL)
    {
        *list = new_node;
        return;
    }

    t_msg *last = *list;
    while (last->next != NULL)
        last = last->next;

    new_node->next = last->next;
    last->next = new_node;
}

void msg_clear(t_msg **list)
{
    if (!list || !(*list))
        return;

    free((*list)->username);
    free((*list)->text);
    free(*list);
    *list = NULL;
}

void msg_clear_list(t_msg **list)
{
    if (list == NULL || *list == NULL)
        return;

    t_msg *node = *list;
    t_msg *next = NULL;

    while (node != NULL)
    {
        next = node->next;
        msg_clear(&node);
        node = next;
    }

    *list = NULL;
}

int msg_list_size(t_msg *list)
{
    if (list == NULL)
        return 0;

    t_msg *head = list;
    int size = 0;
    while (head != NULL)
    {
        head = head->next;
        ++size;
    }
    return size;
}

t_msg *msg_get_by_id(t_msg *list, int id)
{
    while (list)
    {
        if (list->msg_id == id)
            return list;

        list = list->next;
    }
    return NULL;
}

void msg_pop_front(t_msg **head)
{
    if (head == NULL || *head == NULL)
        return;

    if ((*head)->next == NULL)
    {
        msg_clear(head);
        *head = NULL;
        return;
    }

    t_msg *temp = *head;
    *head = (*head)->next;
    msg_clear(&temp);
}

void msg_pop_back(t_msg **head)
{

    if (head == NULL || *head == NULL)
        return;

    if ((*head)->next == NULL)
    {
        msg_clear(head);
        return;
    }

    t_msg *prelast = *head;
    while (prelast->next->next != NULL)
        prelast = prelast->next;
        
    msg_clear(&prelast->next);
    prelast->next = NULL;
}

void msg_pop_index(t_msg **list, int index)
{

    int size = 0;
    t_msg *head = *list;
    while (head != NULL)
    {
        head = head->next;
        ++size;
    }

    if (index <= 0)
    {
        msg_pop_front(list);
        return;
    }
    else if (index >= size)
    {
        msg_pop_back(list);
        return;
    }

    t_msg *current = *list;
    for (int i = 0; current != NULL && i < (index - 1); ++i)
        current = current->next;

    t_msg *next = current->next->next;
    msg_clear(&current->next);
    current->next = next;
}

void msg_pop_id(t_msg **list, int msg_id)
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

t_msg *msg_get_last_msg(t_msg *list)
{
    if (!list)
        return NULL;

    while (list->next)
        list = list->next;

    return list;
}
