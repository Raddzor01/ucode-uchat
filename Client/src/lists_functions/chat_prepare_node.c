#include "../../inc/client.h"

t_chat *chat_prepare_node(int id, char *name, int image_id)
{
    t_chat *new_node = (t_chat *)malloc(sizeof(t_chat));

    new_node->id = id;
    new_node->name = mx_strdup(name);
    new_node->image_id = image_id;
    new_node->messages = get_chat_messages_from_server(id);
    new_node->next = NULL;

    return new_node;
}
