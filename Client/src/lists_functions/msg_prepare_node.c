#include "../../inc/client.h"

t_msg *msg_prepare_node(const int id, const char *message, const time_t time, const int user_id, const char *username)
{
    t_msg *new_node = (t_msg *)malloc(sizeof(t_msg));

    new_node->msg_id = id;
    new_node->text = mx_strdup(message);
    new_node->time = time;
    new_node->user_id = user_id;
    new_node->username = mx_strdup(username);
    new_node->next = NULL;

    return new_node;
}
