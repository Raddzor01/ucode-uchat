#include "../../inc/server.h"

void edit_message(cJSON *json, t_client_info *client_info)
{
    int message_id;
    int chat_id;
    char *new_message;
    char *query;

    message_id = cJSON_GetObjectItem(json, "id")->valueint;
    chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    new_message = cJSON_GetObjectItemCaseSensitive(json, "text")->valuestring;

    query = sqlite3_mprintf("UPDATE messages SET message = '%s' WHERE id = %d AND user_id = %d AND chat_id = %d; ",
                            new_message, message_id, client_info->user->id, chat_id);
    db_execute_query(query);

    sqlite3_free(query);
}
