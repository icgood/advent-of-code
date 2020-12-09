#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

void bitmap_init(struct bitmap_data *data, size_t len) {
	data->len = len;
	data->bitmap_ptr = calloc(len / 8 + 1, sizeof (unsigned char));
}

size_t bitmap_len(struct bitmap_data *data) {
	return data->len;
}

unsigned char *bitmap_pointer(struct bitmap_data *data) {
	return data->bitmap_ptr;
}

void bitmap_set(struct bitmap_data *data, size_t pos) {
	size_t idx = pos / 8;
	size_t bit = 1 << (pos % 8);
	data->bitmap_ptr[idx] |= bit;
}

int bitmap_get(struct bitmap_data *data, size_t pos) {
	size_t idx = pos / 8;
	size_t bit = 1 << (pos % 8);
	return data->bitmap_ptr[idx] & bit ? 1 : 0;
}

void bitmap_free(struct bitmap_data *data) {
	free(data->bitmap_ptr);
}
