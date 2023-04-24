#include "../../inc/client.h"

void chat_clear_single_node(t_chat **list)
{
    if (!list || !(*list))
        return;

    if ((*list)->name)
        mx_strdel(&(*list)->name);
    
    if ((*list)->messages)
        msg_clear_list(&(*list)->messages);

    free(*list);
    *list = NULL;
}
