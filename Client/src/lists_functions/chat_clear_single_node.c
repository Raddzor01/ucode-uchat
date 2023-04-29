#include "../../inc/client.h"

void chat_clear_single_node(t_chat **list)
{
    if (!list || !(*list))
        return;

    if ((*list)->name)
        mx_strdel(&(*list)->name);
    
    if ((*list)->messages)
        msg_clear_list(&(*list)->messages);

    for (int i = 0; (*list)->users[i] != NULL; i++)
    {
        free((*list)->users[i]);
        (*list)->users[i] = NULL;
    }
    free((*list)->users);
    (*list)->users = NULL;

    free(*list);
    *list = NULL;
}
