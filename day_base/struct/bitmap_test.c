#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "bitmap.h"

typedef struct {
	struct bitmap_data bitmap;
	unsigned char data[2];
} static_t;

int main() {
	struct bitmap_data data;
	struct bitmap_data mask;
	static_t data2;

	bitmap_init(&data, 5);
	bitmap_init(&mask, 7);
	bitmap_init_static(&data2.bitmap, 10);

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

	assert(bitmap_cmp(&data, &data2.bitmap) != 0);
	bitmap_set(&data2.bitmap, 3);
	assert(bitmap_cmp(&data, &data2.bitmap) != 0);
	bitmap_set(&data2.bitmap, 7);
	assert(bitmap_cmp(&data, &data2.bitmap) != 0);
	bitmap_set(&data2.bitmap, 9);
	assert(bitmap_cmp(&data, &data2.bitmap) == 0);

	bitmap_set(&mask, 0);
	bitmap_set(&mask, 6);
	for (size_t i=0; i<10; i++) {
		if (i == 3) assert(1 == bitmap_bitand(&data, i, &mask));
		else assert(0 == bitmap_bitand(&data, i, &mask));
	}

	bitmap_free(&mask);
	bitmap_free(&data);
	return 0;
}
