#include "../../inc/client.h"

t_msg *msg_prepare_node(int id, char *message, time_t time, int user_id, char *username)
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
