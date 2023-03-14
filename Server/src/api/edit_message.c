#include "../../inc/server.h"

void edit_message(cJSON *json, t_client_info *client_info) {

    sqlite3 *db;
    sqlite3_stmt *stmt;

    int message_id = cJSON_GetObjectItem(json, "id");
    int chat_id = cJSON_GetObjectItem(json, "chat_id");
    char *new_text = cJSON_GetObjectItemCaseSensitive(json, "text");

    char *query = sqlite3_mprintf("UPDATE messages SET text = '%s' WHERE id = '%d' AND user_id = '%d' AND chat_id = '%d'", new_text, message_id, client_info->user->id, chat_id);

    db = db_open();
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_step(stmt);

    sqlite3_free(query);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}
