#include "../../inc/server.h"

void user_signup(cJSON *json, t_client_info *client_info) {

    db_init();

    const cJSON *username = cJSON_GetObjectItemCaseSensitive(json, "name");
    const cJSON *user_password = cJSON_GetObjectItemCaseSensitive(json, "password");

    char request[MAX_BUF_SIZE];

    sprintf(request, "INSERT INTO users (username, password) VALUES('%s', '%s')", username->valuestring, user_password->valuestring);

    db_execute_request(request);

    send_responde(client_info->ssl, REQ_USER_SIGNUP, ERR_SUCCESS);
}
