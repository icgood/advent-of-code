#include <assert.h>
#include <stdint.h>
#include "queue.h"

typedef struct {
	long int num;
	char letter;
} test;

int main() {
	struct queue_data data;
	test *array;
	size_t idx;

	queue_init(&data, &array, sizeof (test));

	for (int i=0; i<100; i++) {
		idx = queue_push(&data);
		array[idx].num = i;
		array[idx].letter = 'a' + (i % 26);
	}

	for (int i=0; i<50; i++) {
		idx = queue_pop(&data);
		assert(i == array[idx].num);
		assert('a' + (i % 26) == array[idx].letter);
	}

	assert(50 == queue_len(&data));
	for (size_t i=0; i<50; i++) {
		idx = queue_peek(&data, i);
		assert(i + 50 == array[idx].num);
	}

	for (int i=150; i<160; i++) {
		idx = queue_push(&data);
		array[idx].num = i;
		array[idx].letter = 'a' + (i % 26);
	}

	assert(60 == queue_len(&data));
	for (size_t i=50; i<60; i++) {
		idx = queue_peek(&data, i);
		assert(i + 100 == array[idx].num);
	}

	queue_free(&data);
	return 0;
}
