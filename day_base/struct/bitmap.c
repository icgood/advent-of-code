#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

void bitmap_init(struct bitmap_data *data, size_t bits) {
	data->type = BITMAP_DYNAMIC;
	data->size = bits / 8 + 1;
	data->count = 0;
	data->ptr = calloc(data->size, sizeof (unsigned char));
}

void bitmap_init_static(struct bitmap_data *data, size_t bits) {
	// NOTE: this must be used as part of a containing struct where the
	// bitmap data is *immediately* followed by a byte array with at
	// least `after` bits of space.
	data->type = BITMAP_STATIC;
	data->size = (bits + 7) / 8;
	data->count = 0;
	data->ptr = NULL;
	memset(bitmap_pointer(data), 0, data->size);
}

size_t bitmap_len(struct bitmap_data *data) {
	return data->size * 8;
}

size_t bitmap_sizeof(struct bitmap_data *data) {
	return data->size;
}

unsigned char *bitmap_pointer(struct bitmap_data *data) {
	if (data->type == BITMAP_DYNAMIC) return data->ptr;
	else if (data->type == BITMAP_STATIC) return data->buf;
	else assert(!"invalid bitmap type");
}

size_t bitmap_count(struct bitmap_data *data) {
	return data->count;
}

size_t bitmap_min(struct bitmap_data *data, int val) {
	for (size_t i=0; i<bitmap_len(data); i++) {
		if (bitmap_get(data, i) == val) return i;;
	}
	assert(!"no matching bits found");
}

size_t bitmap_max(struct bitmap_data *data, int val) {
	size_t max = 0;
	for (size_t i=1; i<=bitmap_len(data); i++) {
		if (bitmap_get(data, i-1) == val) max = i;
	}
	if (max > 0) return max - 1;
	assert(!"no matching bits found");
}

int bitmap_cmp(struct bitmap_data *data, struct bitmap_data *other) {
	size_t len = bitmap_len(data), other_len = bitmap_len(other);
	size_t max_len = len > other_len ? len : other_len;
	for (size_t i=0; i<max_len; i++) {
		int bit = bitmap_get(data, i);
		int other_bit = bitmap_get(other, i);
		if (!bit && other_bit) return -1;
		else if (bit && !other_bit) return 1;
	}
	return 0;
}

void bitmap_assign(struct bitmap_data *data, size_t pos, int val) {
	if (val) bitmap_set(data, pos);
	else bitmap_unset(data, pos);
}

void bitmap_set(struct bitmap_data *data, size_t pos) {
	const size_t idx = pos / 8;
	const unsigned char bit = 1 << (pos % 8);
	unsigned char *ptr = bitmap_pointer(data);
	if (!bitmap_get(data, pos)) data->count++;
	ptr[idx] |= bit;
}

void bitmap_unset(struct bitmap_data *data, size_t pos) {
	const size_t idx = pos / 8;
	const unsigned char bit = 1 << (pos % 8);
	unsigned char *ptr = bitmap_pointer(data);
	if (bitmap_get(data, pos)) data->count--;
	ptr[idx] &= ~bit;
}

int bitmap_get(struct bitmap_data *data, size_t pos) {
	const size_t idx = pos / 8;
	if (idx >= data->size) return 0;
	const unsigned char bit = 1 << (pos % 8);
	unsigned char *ptr = bitmap_pointer(data);
	return ptr[idx] & bit ? 1 : 0;
}

int bitmap_bitand(struct bitmap_data *data, size_t pos, struct bitmap_data *with) {
	for (size_t i=0; i<bitmap_len(with); i++) {
		if (bitmap_get(with, i) && !bitmap_get(data, pos + i)) return 0;
	}
	return 1;
}

void bitmap_clear(struct bitmap_data *data) {
	unsigned char *ptr = bitmap_pointer(data);
	memset(ptr, 0, data->size);
	data->count = 0;
}

void bitmap_resize(struct bitmap_data *data, size_t bits) {
	assert(data->type == BITMAP_DYNAMIC);
	const size_t needed_size = bits / 8 + 1;
	size_t new_size = data->size;
	while (new_size < needed_size) new_size *= 2;
	if (new_size > data->size) {
		unsigned char *old_ptr = data->ptr;
		data->ptr = calloc(new_size, sizeof (unsigned char));
		memcpy(data->ptr, old_ptr, data->size);
		free(old_ptr);
		data->size = new_size;
	}
}

void bitmap_free(struct bitmap_data *data) {
	assert(data->type == BITMAP_DYNAMIC);
	free(data->ptr);
}
