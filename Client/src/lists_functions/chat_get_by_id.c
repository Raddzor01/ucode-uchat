#include "../../inc/client.h"

t_chat *chat_get_chat_by_id(t_chat *list, int chat_id)
{
    while (list)
    {
        if (list->id == chat_id)
            return list;

        list = list->next;
    }

    return NULL;
}
