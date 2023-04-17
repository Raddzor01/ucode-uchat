#include "../inc/client.h"

void chat_push_back(t_chat **list, t_chat *new_node)
{
    if (list != NULL && *list == NULL)
    {
        *list = new_node;
        return;
    }

    t_chat *last = *list;
    while (last->next != NULL)
        last = last->next;

    new_node->next = last->next;
    last->next = new_node;
}

void chat_push_front(t_chat **list, t_chat *new_node)
{
    t_chat *new = new_node;
    new->next = *list;
    *list = new;
}

t_chat *chat_get_by_id(t_chat *list, int chat_id)
{
    while (list)
    {
        if (list->id == chat_id)
            return list;

        list = list->next;
    }
    return NULL;
}

void clear_chat(t_chat **list)
{
    if (!list || !(*list))
        return;

    if ((*list)->name)
        free((*list)->name);

    free(*list);
    *list = NULL;
}

void chat_clear_list(t_chat **list)
{
    if (list == NULL || *list == NULL)
        return;

    t_chat *node = *list;
    t_chat *next = NULL;

    while (node)
    {
        next = node->next;
        msg_clear_list(&node->messages);
        clear_chat(&node);
        node = next;
    }

    *list = NULL;
}

int chat_list_size(t_chat *list)
{
    int size = 0;
    if (list == NULL)
        return 0;

    t_chat *head = list;
    while (head != NULL)
    {
        head = head->next;
        size++;
    }
    return size;
}
