#include "../../inc/server.h"

int db_get_id_by_username(char *username)
{
    sqlite3 *db = db_open();
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "SELECT id FROM users WHERE username = ?; ", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, -1, NULL);

    int user_id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        user_id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return user_id;
}
