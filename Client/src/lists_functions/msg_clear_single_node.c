#include "../../inc/client.h"

void msg_clear_single_node(t_msg **node)
{
    if (!node || !(*node))
        return;

    mx_strdel(&(*node)->username);
    mx_strdel(&(*node)->text);

    free(*node);
    *node = NULL;
}
