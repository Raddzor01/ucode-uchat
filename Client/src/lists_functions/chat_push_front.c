#include "../../inc/client.h"

void chat_push_front(t_chat **list, t_chat *new_node)
{
    t_chat *new = new_node;
    new->next = *list;
    *list = new;
}
