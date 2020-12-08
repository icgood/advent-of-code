#include <stdlib.h>
#include <string.h>
#include "array.h"

void array_init(struct array_data *data, void *array_ptr, size_t entry_size) {
	data->len = 0;
	data->size = 2;
	data->entry_size = entry_size;
	data->array_ptr = (void **) array_ptr;
	*data->array_ptr = calloc(2, entry_size);
}

size_t array_len(struct array_data *data) {
	return data->len;
}

void *array_pointer(struct array_data *data) {
	return *data->array_ptr;
}

size_t array_add(struct array_data *data) {
	size_t index = data->len;
	if (++data->len >= data->size) {
		size_t entry_size = data->entry_size;
		void *old_array_ptr = *data->array_ptr;
		void *new_array_ptr = *data->array_ptr = calloc(data->size *= 2, entry_size);
		memcpy(new_array_ptr, old_array_ptr, data->len * entry_size);
		free(old_array_ptr);
	}
	return index;
}

void array_free(struct array_data *data) {
	free(*data->array_ptr);
}
