#include "../inc/server.h"

t_req_type handle_request(t_client_info *client_info, char *request)
{
    cJSON *json;
    t_req_type type;
    t_map_entry *entery;

    json = cJSON_Parse(request);
    if (json == NULL)
    {
        mx_logs("parse error", INFO_LOG);
        cJSON_Delete(json);
        return 1;
    }

    type = cJSON_GetObjectItem(json, "type")->valueint;

    if (type == REQ_EXIT || type == REQ_LOGOUT)
    {
        cJSON_Delete(json);
        return type;
    }

    if (type > MAP_SIZE)
    {
        mx_logs("Unknown request type", ERROR_LOG);
        cJSON_Delete(json);
        return REQ_UNKNOWN;
    }

    entery = &request_map[type];
    entery->handler(json, client_info);

    cJSON_Delete(json);
    return type;
}
