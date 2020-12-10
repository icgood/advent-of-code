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
	array_resize(data, index+1);
	return index;
}

void array_resize(struct array_data *data, size_t len) {
	size_t new_size = data->size;
	while (len > new_size) new_size *= 2;
	if (new_size > data->size) {
		size_t entry_size = data->entry_size;
		void *old_array_ptr = *data->array_ptr;
		void *new_array_ptr = *data->array_ptr = calloc(new_size, entry_size);
		memcpy(new_array_ptr, old_array_ptr, data->len * entry_size);
		free(old_array_ptr);
		data->size = new_size;
	}
	data->len = len;
}

void array_free(struct array_data *data) {
	free(*data->array_ptr);
}
