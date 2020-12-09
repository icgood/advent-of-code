#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

void bitmap_init(struct bitmap_data *data, size_t len) {
	data->size = len / 8 + 1;
	if (data->size == 0) data->size = 1;
	data->bitmap_ptr = calloc(data->size, sizeof (unsigned char));
}

size_t bitmap_len(struct bitmap_data *data) {
	return data->size * 8;
}

unsigned char *bitmap_pointer(struct bitmap_data *data) {
	return data->bitmap_ptr;
}

size_t bitmap_count(struct bitmap_data *data) {
	size_t count = 0;
	for (size_t i=0; i<bitmap_len(data); i++) {
		if (bitmap_get(data, i)) count++;
	}
	return count;
}

void bitmap_set(struct bitmap_data *data, size_t pos) {
	size_t idx = pos / 8;
	size_t bit = 1 << (pos % 8);
	data->bitmap_ptr[idx] |= bit;
}

void bitmap_unset(struct bitmap_data *data, size_t pos) {
	size_t idx = pos / 8;
	size_t bit = 1 << (pos % 8);
	data->bitmap_ptr[idx] &= ~bit;
}

int bitmap_get(struct bitmap_data *data, size_t pos) {
	size_t idx = pos / 8;
	size_t bit = 1 << (pos % 8);
	return data->bitmap_ptr[idx] & bit ? 1 : 0;
}

void bitmap_resize(struct bitmap_data *data, size_t len) {
	size_t needed_size = len / 8 + 1;
	size_t new_size = data->size;
	while (new_size < needed_size) new_size *= 2;
	if (new_size > data->size) {
		unsigned char *old_ptr = data->bitmap_ptr;
		data->bitmap_ptr = calloc(new_size, sizeof (unsigned char));
		memcpy(data->bitmap_ptr, old_ptr, data->size);
		free(old_ptr);
		data->size = new_size;
	}
}

void bitmap_free(struct bitmap_data *data) {
	free(data->bitmap_ptr);
}
