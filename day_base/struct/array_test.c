#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "array.h"

typedef struct {
	uint64_t num;
	uint8_t letter;
} test;

int main() {
	struct array_data data;
	test *array;

	array_init(&data, &array, sizeof (test));
	for (uint64_t i=0; i<100; i++) {
		size_t idx = array_add(&data);
		array[idx].num = i;
		array[idx].letter = 'a' + (i % 26);
	}

	uint64_t total = 0;
	for (size_t idx=0; idx<array_len(&data); idx++) {
		total += array[idx].num;
		assert('a' + (idx % 26) == array[idx].letter);
	}
	assert(4950 == total);

	array_free(&data);
	return 0;
}
