#include "libmx.h"

void *mx_realloc(void *ptr, size_t size) {
    void *new_data = NULL;
    if(size) {
		if(!ptr)
			return (void *)malloc(size);

		new_data = (void *)malloc(size);
		if(new_data) {
			mx_memcpy(new_data, ptr, size);
			free(ptr);
		}
	}
    return new_data;
}
