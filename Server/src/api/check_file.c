#include "../../inc/server.h"

void check_file(cJSON *json, t_client_info *client_info)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char *query = NULL;

	char *filename = NULL;
	long file_size = 0;

	cJSON *responde_json = NULL;
	char *json_str = NULL;

	int image_id = cJSON_GetObjectItemCaseSensitive(json, "image_id")->valueint;

	db = db_open();
	query = sqlite3_mprintf("SELECT filename, file_size FROM files "
							"WHERE id = %d; ",
							image_id);
	stmt = db_execute_query_and_return_stmt(query, db);

	filename = mx_strdup((const char *)sqlite3_column_text(stmt, 0));
	file_size = sqlite3_column_int64(stmt, 1);

	responde_json = cJSON_CreateObject();

	cJSON_AddNumberToObject(responde_json, "type", REQ_SEND_FILE_TO_CLIENT);
	cJSON_AddStringToObject(responde_json, "filename", filename);
	cJSON_AddNumberToObject(responde_json, "size", file_size);
	json_str = cJSON_PrintUnformatted(responde_json);

	SSL_write(client_info->ssl, json_str, mx_strlen(json_str));

	mx_strdel(&filename);
	mx_strdel(&json_str);
	cJSON_Delete(responde_json);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}
