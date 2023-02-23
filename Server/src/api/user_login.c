#include "../../inc/server.h"

void user_login(cJSON *json, t_client_info *client_info) {

    json++;
    client_info++;
    db_init();

    cJSON *username = cJSON_GetObjectItemCaseSensitive(json, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(json, "password");

}
