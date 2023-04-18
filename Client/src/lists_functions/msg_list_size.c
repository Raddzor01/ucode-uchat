#include "../../inc/client.h"

int msg_list_size(t_msg *list)
{
    int size = 0;

    if (list == NULL)
        return 0;

    t_msg *head = list;
    while (head != NULL)
    {
        head = head->next;
        size++;
    }
    
    return size;
}
