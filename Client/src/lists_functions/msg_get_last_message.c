#include "../../inc/client.h"

t_msg *msg_get_last_message(t_msg *list)
{
    if (!list )
        return NULL;

    while (list->next)
        list = list->next;

    return list;
}

