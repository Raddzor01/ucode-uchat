#include "../../inc/client.h"

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
