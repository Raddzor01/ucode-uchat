#include "../../inc/server.h"

void delete_account(cJSON *__attribute__((unused)) json, t_client_info *client_info)
{
    char *query = NULL;

    // bruh this shit didnt work
    // query = sqlite3_mprintf("DELETE users, messages, members "
    //                         "FROM users "
    //                         "LEFT JOIN messages ON messages.user_id = users.id "
    //                         "LEFT JOIN members ON members.user_id = users.id "
    //                         "WHERE users.id = %d ; ",

    query = sqlite3_mprintf("DELETE FROM users WHERE id = %d; ",
                            client_info->user->id);
    db_execute_query(query);

    sqlite3_free(query);

    query = sqlite3_mprintf("DELETE FROM messages WHERE user_id = %d; ",
                            client_info->user->id);
    db_execute_query(query);

    sqlite3_free(query);

    query = sqlite3_mprintf("DELETE FROM members WHERE user_id = %d; ",
                            client_info->user->id);
    db_execute_query(query);

    sqlite3_free(query);

    send_responde(client_info->ssl, REQ_DEL_ACCOUNT, ERR_SUCCESS);
    user_logout(json, client_info);
}
