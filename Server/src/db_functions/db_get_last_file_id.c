#include "../../inc/server.h"

int db_get_last_file_id(char *filename)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *query = NULL;
    int file_id;

    db = db_open();
    query = sqlite3_mprintf("SELECT id FROM files WHERE filename = '%s' "
                            "ORDER BY id DESC LIMIT 1; ",
                            filename);
    stmt = db_execute_query_and_return_stmt(query, db);

    file_id = sqlite3_column_int(stmt, 0);

    sqlite3_free(query);
    sqlite3_close(db);
    sqlite3_finalize(stmt);

    return file_id;
}
