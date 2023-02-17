#include "../inc/server.h"

void request_switch(cJSON *json, t_client_info *client_info) {

    t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

    switch(type) {
        case REQ_TEST_RESPONDE:
            mx_logs(cJSON_Print(json), INFO_LOG);
            break;
        case REQ_USER_SIGNUP:
            user_signup(json, client_info);
            break;
        case REQ_USER_LOGIN:
            user_login(json, client_info);
            break;
        default:
            mx_logs("Unknown request type", INFO_LOG);
            break;
    }
}

t_req_type handle_request(t_client_info *client_info, char *request) {

    cJSON *json;
    if((json = cJSON_Parse(request)) == NULL) {
        mx_logs("parse error", INFO_LOG);
        return 1;
    }

    t_req_type type = cJSON_GetObjectItem(json, "type")->valueint;

    if((type == REQ_EXIT) || (type == REQ_LOGOUT)) {
        cJSON_Delete(json);
        return type;
    }

    request_switch(json, client_info);

    cJSON_Delete(json);
    return type;

}
