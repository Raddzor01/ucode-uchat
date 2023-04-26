#include "../../inc/server.h"

void update_user_photo(cJSON *json, t_client_info *client_info)
{
    char *query = NULL;
    int file_id = 0;

    char *extension = mx_strdup(cJSON_GetObjectItemCaseSensitive(json, "extension")->valuestring);
    long time = cJSON_GetObjectItemCaseSensitive(json, "time")->valueint;
    long size = cJSON_GetObjectItemCaseSensitive(json, "size")->valueint;

    char *file_indf = mx_strjoin(mx_ltoa(time), mx_itoa(client_info->user->id));
    char *file_name = mx_strjoin(file_indf, extension);
    char *file_path = mx_strjoin(DATA_DIR, file_name);

    if(write_file_to_data(file_path, size, client_info->ssl))
        return;
    
    query = sqlite3_mprintf("INSERT INTO files (filename, file_size, file_path) VALUES('%s', %d, '%s'); ",
                            file_name, size, file_path);
    db_execute_query(query);
    sqlite3_free(query);
    
    file_id = db_get_last_file_id(file_name);

    query = sqlite3_mprintf("UPDATE users SET image_id = %d WHERE id = %d; ",
                            file_id, client_info->user->id);
    db_execute_query(query);

    mx_strdel(&extension);
    mx_strdel(&file_indf);
    mx_strdel(&file_name);
    mx_strdel(&file_path);
    sqlite3_free(query);
}
