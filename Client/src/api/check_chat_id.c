#include "../../inc/client.h"

int check_chat_id_from_server()
{
    char *json_str = read_from_server();
    cJSON *json = cJSON_Parse(json_str);

    if (json == NULL)
    {
        mx_printerr("Error getting new chat ID\ncJSON is NULL\n");
        cJSON_Delete(json);
        free(json_str);
        return -1;
    }

    int chat_id = cJSON_GetObjectItem(json, "chat_id")->valueint;

    cJSON_Delete(json);
    free(json_str);
    
    return chat_id;
}
