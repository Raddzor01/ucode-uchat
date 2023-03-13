#include "../inc/client.h"

int send_sign_up_to_server(const char *username, const char *password) {

  cJSON *json = cJSON_CreateObject();

  cJSON_AddNumberToObject(json, "type", REQ_USER_SIGNUP);
  cJSON_AddStringToObject(json, "username", username);
  cJSON_AddStringToObject(json, "password", password);

  char *json_str = cJSON_PrintUnformatted(json);
  // printf("%d\n", SSL_write(ssl, json_str, mx_strlen(json_str)));
  SSL_write(info->ssl, json_str, mx_strlen(json_str));

  cJSON_Delete(json);
  free(json_str);

  return check_account_exists();
}

int send_login_to_server(const char *username, const char *password) {

  cJSON *json = cJSON_CreateObject();

  cJSON_AddNumberToObject(json, "type", REQ_USER_LOGIN);
  cJSON_AddStringToObject(json, "username", username);
  cJSON_AddStringToObject(json, "password", password);

  char *json_str = cJSON_PrintUnformatted(json);
  SSL_write(info->ssl, json_str, mx_strlen(json_str));

  cJSON_Delete(json);
  free(json_str);

  return check_account_from_server();
}

int send_message_to_server(const char *str) {
  // printf("%d\t%s\n", account->id, account->username);

  cJSON *json = cJSON_CreateObject();

  cJSON_AddNumberToObject(json, "type", REQ_SEND_MSG);
  cJSON_AddNumberToObject(json, "user_id", account->id);
  cJSON_AddNumberToObject(json, "chat_id", account->chat_id);
  cJSON_AddStringToObject(json, "message", str);
  cJSON_AddStringToObject(json, "username", account->username);
  cJSON_AddNumberToObject(json, "time", time(NULL));

  char *json_str = cJSON_PrintUnformatted(json);
  SSL_write(info->ssl, json_str, mx_strlen(json_str));

  cJSON_Delete(json);
  free(json_str);

  return 0;
}

char *send_from_server() {

  char buffer[1024];
  int bytes = SSL_read(info->ssl, buffer, sizeof(buffer));

  mx_logs(buffer, INFO_LOG);

  if (bytes > 0) {
    buffer[bytes] = 0;
    return mx_strdup(buffer);
  }
  return NULL;
}

int send_exit_from_server() {

  cJSON *json = cJSON_CreateObject();

  cJSON_AddNumberToObject(json, "type", REQ_EXIT);

  char *json_str = cJSON_PrintUnformatted(json);
  SSL_write(info->ssl, json_str, mx_strlen(json_str));

  cJSON_Delete(json);
  free(json_str);

  return 0;
}

int get_user_chats() {

  cJSON *json = cJSON_CreateObject();
  cJSON_AddNumberToObject(json, "type", REQ_GET_CHATS);

  char *json_str = cJSON_PrintUnformatted(json);
  SSL_write(info->ssl, json_str, mx_strlen(json_str));

  cJSON_Delete(json);
  free(json_str);

  return 0;
}

int create_chat_in_server(const char *chat_name, int chat_type) {
  // printf("%d\t%s\n", account->id, account->username);

  cJSON *json = cJSON_CreateObject();

  cJSON_AddNumberToObject(json, "type", REQ_CREATE_CHAT);
  cJSON_AddStringToObject(json, "name", chat_name);
  cJSON_AddNumberToObject(json, "user_id", account->id);
  cJSON_AddNumberToObject(json, "chat_type", chat_type);
  cJSON_AddNumberToObject(json, "time", time(NULL));

  char *json_str = cJSON_PrintUnformatted(json);
  SSL_write(info->ssl, json_str, mx_strlen(json_str));

  cJSON_Delete(json);
  free(json_str);

  return 0;
}

int check_account_exists() {

  char *str = send_from_server();
  cJSON *json = cJSON_Parse(str);

  if (json == NULL) {
    mx_printerr("cJSON is NULL\n");
    return -1;
  }

  t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
  // t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

  // if (type == REQ_USER_SIGNUP) {
  // 	mx_printerr("Error in type\n");
  // 	return NULL;
  // }

  // printf("%d\n", error);

  return error;
}

bool check_account_from_server() {

  char *str = send_from_server();
  cJSON *json = cJSON_Parse(str);

  if (json == NULL) {
    mx_printerr("cJSON is NULL\n");
    return 0;
  }

  t_error_type error = cJSON_GetObjectItem(json, "error_code")->valueint;
  // t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

  // add error checks
  if (error != 0) {
    mx_printerr("Error in account cJSON\n");
    return 0;
  }

  account->id = cJSON_GetObjectItem(json, "id")->valueint;
  account->username = cJSON_GetObjectItem(json, "username")->valuestring;
  // char *password = cJSON_GetObjectItem(json, "password")->valuestring;
  // printf("%d\t%s\n", account->id, account->username);
  cJSON_Delete(json);
  free(str);

  // ???
  return 1;
}

int check_chat_id_from_server() {

  char *str = send_from_server();
  cJSON *json = cJSON_Parse(str);

  if (json == NULL) {
    mx_printerr("cJSON is NULL\n");
    return -1;
  }

  int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

  return chat_id;
}
