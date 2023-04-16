#include "../../inc/server.h"

void edit_password(cJSON *json, t_client_info *client_info)
{
    char *query = NULL;
    char *new_password = cJSON_GetObjectItemCaseSensitive(json, "new_password")->valuestring;

    query = sqlite3_mprintf("UPDATE users SET password = '%s' WHERE id = %d; ", 
                            new_password, client_info->user->id);

    db_execute_query(query);
    
    send_responde(client_info->ssl, REQ_EDIT_USERNAME, ERR_SUCCESS);
}

