#include <assert.h>
#include <stdio.h>
#include "deque.h"

typedef struct {
	long int num;
	char letter;
} test;

int main() {
	struct deque_data deque;
	test *array;
	size_t idx;

	deque_init(&deque, &array, sizeof (test));

	for (int i=0; i<100; i++) {
		idx = deque_add(&deque, DEQUE_BACK);
		array[idx].num = i;
		array[idx].letter = 'a' + (i % 26);
	}

	for (int i=0; i<50; i++) {
		idx = deque_remove(&deque, DEQUE_FRONT);
		assert(i == array[idx].num);
		assert('a' + (i % 26) == array[idx].letter);
	}

	assert(50 == deque_len(&deque));
	for (size_t i=0; i<50; i++) {
		idx = deque_peek(&deque, i, DEQUE_FRONT);
		assert(i + 50 == array[idx].num);
	}

	for (int i=150; i<160; i++) {
		idx = deque_add(&deque, DEQUE_BACK);
		array[idx].num = i;
		array[idx].letter = 'a' + (i % 26);
	}

	assert(60 == deque_len(&deque));
	for (size_t i=50; i<60; i++) {
		idx = deque_peek(&deque, i, DEQUE_FRONT);
		assert(i + 100 == array[idx].num);
	}

	deque_free(&deque);
	return 0;
}
