#include "../../inc/server.h"

void edit_username(cJSON *json, t_client_info *client_info)
{
    char *query = NULL;
    char *new_username = cJSON_GetObjectItemCaseSensitive(json, "new_username")->valuestring;

    if (db_check_user_exists(new_username))
    {
        send_responde(client_info->ssl, REQ_EDIT_USERNAME, ERR_USER_EXISTS);
        return;
    }

    query = sqlite3_mprintf("UPDATE users SET username = '%s' WHERE id = %d; ",
                            new_username, client_info->user->id);
    db_execute_query(query);

    send_responde(client_info->ssl, REQ_EDIT_USERNAME, ERR_SUCCESS);
}
