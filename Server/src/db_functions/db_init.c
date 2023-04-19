#include "../../inc/server.h"

int db_init()
{
    FILE *fp;
    char *error = 0;
    char ch;
    char buf[MAX_BUF_SIZE];

    if (db_check_exist() == 1)
        return 0;

    mx_logs("Generating database file...", INFO_LOG);
    sqlite3 *db = db_open();

    fp = fopen(SQL_NAME, "r");
    if (fp == NULL)
    {
        mx_logs("Cannot open sql file", ERROR_LOG);
        return 1;
    }

    int i;
    for (i = 0; (ch = fgetc(fp)) != EOF; i++)
        buf[i] = ch;
    buf[i + 1] = '\0';

    if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, 0, &error) != SQLITE_OK)
    {
        mx_logs(error, ERROR_LOG);
        sqlite3_free(error);
        return 1;
    }

    if (sqlite3_exec(db, buf, NULL, 0, &error) != SQLITE_OK)
    {
        mx_logs(error, ERROR_LOG);
        sqlite3_free(error);
        return 1;
    }

    if (sqlite3_exec(db, "INSERT INTO files (filename, file_type, file_size, file_path, time) VALUES('default_avatar', '.png', 14171, 'Server/db/data/default_avatar.png', 0); ", NULL, 0, &error) != SQLITE_OK)
    {
        mx_logs(error, ERROR_LOG);
        sqlite3_free(error);
        return 1;
    }


    sqlite3_close(db);
    return 0;
}
