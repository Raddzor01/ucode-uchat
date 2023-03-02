#include "../inc/server.h"

int db_check_exist()
{

    struct stat info;

    if (stat(DB_NAME, &info) == 0)
    {
        return 1;
    }

    return 0;
}

sqlite3 *db_open()
{

    sqlite3 *db;

    if (sqlite3_open(DB_NAME, &db) != SQLITE_OK)
    {
        mx_logs(strerror(errno), ERROR_LOG);
        return NULL;
    }

    return db;
}

int db_init()
{

    FILE *fp;
    char *error = 0;
    char ch;
    char buf[MAX_BUF_SIZE];

    if (db_check_exist() == 1)
    {
        return 0;
    }

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
    {
        buf[i] = ch;
    }
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

    sqlite3_close(db);
    return 0;
}

int db_execute_request(char *request)
{

    sqlite3 *db = db_open();

    char *error;
    if (sqlite3_exec(db, request, NULL, NULL, &error) != SQLITE_OK)
    {
        mx_logs(error, ERROR_LOG);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

int db_check_user_exists(char *username)
{

    char buff[MAX_BUF_SIZE];
    sprintf(buff, "SELECT EXISTS (SELECT id FROM users WHERE username = '%s')", username);

    sqlite3 *db = db_open();
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, buff, -1, &stmt, NULL);
    sqlite3_step(stmt);
    int user_exists = sqlite3_column_int64(stmt, 0);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return user_exists == 1;
}

int db_get_id_by_username(char *username)
{

    sqlite3 *db = db_open();
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, "SELECT `id` FROM `users` WHERE `username` = ?", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, username, -1, NULL);

    int user_id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        user_id = sqlite3_column_int64(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return user_id;

}
