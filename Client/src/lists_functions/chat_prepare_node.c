#include "../../inc/client.h"

t_chat *chat_prepare_node(const int id, const char *name, const int image_id, const int user_privilege, int chat_type)
{
    t_chat *new_node = (t_chat *)malloc(sizeof(t_chat));

    new_node->id = id;
    new_node->chat_type = chat_type;
    new_node->user_privilege = user_privilege;
    new_node->name = mx_strdup(name);
    new_node->image_id = image_id;
    new_node->messages = NULL;
    new_node->next = NULL;

    return new_node;
}
