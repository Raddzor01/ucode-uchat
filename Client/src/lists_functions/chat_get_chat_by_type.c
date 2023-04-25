#include "../../inc/client.h"

t_chat *chat_get_chat_by_type(t_chat *list, int type)
{
    while (list)
    {
        if (list->chat_type == type)
            return list;

        list = list->next;
    }

    return NULL;
}

