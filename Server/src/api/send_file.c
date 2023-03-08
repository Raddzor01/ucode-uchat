#include "../../inc/server.h"

int receive_file_from_client(SSL *ssl, sqlite3 *db) {
  char filename[256];
  int filename_len;
  size_t file_size;
  int bytes_received;
  unsigned char buffer[4096];
  FILE *fp;

  // Чтение имени файла и его размера
//   if (SSL_read(ssl, &filename_len, sizeof(int)) <= 0) {
//     return -1;
//   }
//   if (SSL_read(ssl, filename, filename_len) <= 0) {
//     return -1;
//   }
//   filename[filename_len] = '\0';
//   if (SSL_read(ssl, &file_size, sizeof(size_t)) <= 0) {
//     return -1;
//   }

//   // Проверка на максимальный размер файла
//   if (file_size > MAX_FILE_SIZE) {
//     return -1;
//   }

  // Создание нового файла на сервере
  fp = fopen(filename, "wb");
  if (fp == NULL) {
    return -1;
  }

  // Прием содержимого файла
  bytes_received = 0;
  while (bytes_received < file_size) {
    int bytes_to_receive = sizeof(buffer);
    if (bytes_received + bytes_to_receive > file_size) {
      bytes_to_receive = file_size - bytes_received;
    }
    int bytes_read = SSL_read(ssl, buffer, bytes_to_receive);
    if (bytes_read <= 0) {
      fclose(fp);
      return -1;
    }
    fwrite(buffer, 1, bytes_read, fp);
    bytes_received += bytes_read;
  }

  // Закрытие файла и запись информации в базу данных
  fclose(fp);
  char sql[512];
  snprintf(sql, sizeof(sql), "INSERT INTO files (filename, size) VALUES ('%s', %ld)", filename, file_size);
  int rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
  if (rc != SQLITE_OK) {
    return -1;
  }

  return 0;
}

int add_file_to_db(const char *file_path)
{

    sqlite3 *db = db_open();

    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return 1;
    }

    // Получаем размер файла
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Считываем файл в буфер
    char *buffer = (char *)malloc(fsize + 1);
    fread(buffer, fsize, 1, fp);
    fclose(fp);
    buffer[fsize] = '\0';

    // Вставляем запись в таблицу files
    char *sql = "INSERT INTO files (filename, file_type, file_size, file_content) VALUES (?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing SQL: %s\n", sqlite3_errmsg(db));
        free(buffer);
        return 1;
    }

    // Устанавливаем параметры запроса
    sqlite3_bind_text(stmt, 1, file_path, strlen(file_path), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, "unknown", 7, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, fsize);
    sqlite3_bind_blob(stmt, 4, buffer, fsize, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Error inserting record: %s\n", sqlite3_errmsg(db));
        free(buffer);
        return 1;
    }

    sqlite3_finalize(stmt);
    free(buffer);
    return 0;
}

void send_file(cJSON *json, t_client_info *client_info)
{

    db_init();

    sqlite3 *db = db_open();
    sqlite3_stmt *stmt;

    // int user_id = cJSON_GetObjectItem(json, "user_id")->valueint;
    // int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;
    // char *message = cJSON_GetObjectItemCaseSensitive(json, "message")->valuestring;
    char *file = cJSON_GetObjectItemCaseSensitive(json, "file")->valuestring;
    char *file_name = cJSON_GetObjectItemCaseSensitive(json, "file_name")->valuestring;
    // unsigned long time = cJSON_GetObjectItemCaseSensitive(json, "time")->valueint;

    // int file_size = mx_strlen(file);
    // char *buffer = (char *)malloc(file_size + 1);
    // memcpy(buffer, file, file_size);
    // buffer[file_size] = '\0';

    char *file_dir = mx_strjoin(DATA_DIR, file_name);

    receive_file_from_client(client_info->ssl, db);

    // FILE *fp = fopen(file_dir, "wb");
    // if (fp == NULL)
    // {
    //     char string_to_log[MAX_BUF_SIZE];
    //     sprintf("Error opening file: %s", file_dir);
    //     mx_logs(string_to_log, ERROR_LOG);
    //     free(buffer);
    //     return;
    // }
    // fwrite(buffer, file_size, 1, fp);
    // fclose(fp);

    // int rc = add_file_to_db(file_dir);
    // if (rc != 0)
    // {
    //     fprintf(stderr, "Error adding file to database.\n");
    //     free(buffer);
    //     return;
    // }

    // free(buffer);
}
