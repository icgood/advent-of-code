#include <assert.h>
#include <stdio.h>
#include "array.h"

typedef unsigned long int number_t;

typedef struct {
	number_t num;
	char letter;
} test;

void test_basic_operations() {
	struct array_data data;
	test *array;

	array_init(&data, &array, sizeof (test));
	for (number_t i=0; i<90; i++) {
		size_t idx = array_add(&data);
		array[idx].num = i;
		array[idx].letter = 'a' + (i % 26);
	}

	array_resize(&data, 100);
	for (size_t idx=90; idx<100; idx++) {
		array[idx].num = idx;
		array[idx].letter = 'a' + (idx % 26);
	}

	number_t total = 0;
	for (size_t idx=0; idx<array_len(&data); idx++) {
		total += array[idx].num;
		assert('a' + (idx % 26) == array[idx].letter);
	}

	assert(4950 == total);

	array_free(&data);
}

void test_move() {
	struct array_data data, dest_data;
	test *array, *dest_array;

	array_init(&data, &array, sizeof (test));
	for (number_t i=0; i<100; i++) {
		size_t idx = array_add(&data);
		array[idx].num = i + 33;
		array[idx].letter = 'a' + (i % 26);
	}
	assert(100 == array_len(&data));

	array_move(&data, &dest_data, &dest_array);
	size_t idx = array_add(&dest_data);
	dest_array[idx].num = 133;
	dest_array[idx].letter = 'w';

	assert(101 == array_len(&dest_data));
	for (size_t idx=0; idx<101; idx++) {
		assert(dest_array[idx].num == idx + 33);
		assert(dest_array[idx].letter == 'a' + (idx % 26));
	}

	array_free(&dest_data);
}

int main() {
	test_basic_operations();
	test_move();
	return 0;
}
