#include "../inc/client.h"

void *server_update_thread()
{
    while (true)
    {
        if (!account)
			break;
        
        t_chat *chat = account->chats;
        while (chat)
        {
            int message_id = get_last_msg_from_server(chat->id);
            if (message_id <= 0)
            {
                chat = chat->next;
                continue;
            }
            bool is_current = account->current_chat && account->current_chat->id == chat->id;
            chat = chat->next;
            g_usleep(500000);
        }
        
    }

    return NULL;
}
