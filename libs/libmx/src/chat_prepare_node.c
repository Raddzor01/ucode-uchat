#include "../inc/libmx.h"

t_chat *chat_prepare_node(const int id, const char *name, const int image_id)
{
    t_chat *new_node = (t_chat *)malloc(sizeof(t_chat));

    new_node->id = id;
    new_node->name = mx_strdup(name);
    new_node->image_id = image_id;
    new_node->next = NULL;

    return new_node;
}
