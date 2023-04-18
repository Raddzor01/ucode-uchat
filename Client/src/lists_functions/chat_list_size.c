#include "../../inc/client.h"

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
