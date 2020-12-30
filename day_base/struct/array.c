#include <stdlib.h>
#include <string.h>
#include "array.h"

void array_init(struct array_data *data, void *array_ptr, size_t entry_size) {
	data->len = 0;
	data->size = 8;
	data->entry_size = entry_size;
	data->array_ptr = (void **) array_ptr;
	if (entry_size > 0) *data->array_ptr = calloc(data->size, entry_size);
}

void array_move(struct array_data *data, struct array_data *dest_data, void *dest_array_ptr) {
	if (data != dest_data) *dest_data = *data;
	dest_data->array_ptr = (void **) dest_array_ptr;
	if (data->entry_size > 0) *dest_data->array_ptr = array_pointer(data);
}

size_t array_len(struct array_data *data) {
	return data->len;
}

size_t array_sizeof(struct array_data *data) {
	return data->size * data->entry_size;
}

void *array_pointer(struct array_data *data) {
	return *data->array_ptr;
}

size_t array_add(struct array_data *data) {
	size_t index = data->len;
	array_resize(data, index+1);
	return index;
}

void array_realloc(struct array_data *data, size_t size_hint) {
	size_t entry_size = data->entry_size;
	if (entry_size == 0) return;
	size_t new_size = data->size;
	while (size_hint > new_size) new_size *= 4;
	if (new_size > data->size) {
		void *old_array_ptr = *data->array_ptr;
		void *new_array_ptr = *data->array_ptr = calloc(new_size, entry_size);
		memcpy(new_array_ptr, old_array_ptr, data->len * entry_size);
		free(old_array_ptr);
		data->size = new_size;
	}
}

void array_resize(struct array_data *data, size_t len) {
	array_realloc(data, len);
	data->len = len;
}

void array_free(struct array_data *data) {
	if (data->entry_size > 0) free(*data->array_ptr);
}
