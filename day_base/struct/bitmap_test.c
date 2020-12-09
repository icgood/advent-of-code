#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "bitmap.h"

typedef struct {
	uint64_t num;
	uint8_t letter;
} test;

int main() {
	struct bitmap_data data;

	bitmap_init(&data, 5);

	bitmap_set(&data, 3);
	bitmap_set(&data, 7);
	bitmap_resize(&data, 10);
	bitmap_set(&data, 8);
	bitmap_unset(&data, 8);
	bitmap_set(&data, 9);

	assert(16 == bitmap_len(&data));
	assert(3 == bitmap_count(&data));

	assert(0 == bitmap_get(&data, 0));
	assert(0 == bitmap_get(&data, 1));
	assert(0 == bitmap_get(&data, 2));
	assert(1 == bitmap_get(&data, 3));
	assert(0 == bitmap_get(&data, 4));
	assert(0 == bitmap_get(&data, 5));
	assert(0 == bitmap_get(&data, 6));
	assert(1 == bitmap_get(&data, 7));
	assert(0 == bitmap_get(&data, 8));
	assert(1 == bitmap_get(&data, 9));

	bitmap_free(&data);
	return 0;
}
