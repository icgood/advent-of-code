#include <assert.h>
#include <stdio.h>
#include "deque.h"

typedef unsigned long int number_t;

typedef struct {
	number_t num;
	char letter;
} test_t;

int matcher(void *value_ptr, void *arg) {
	char letter = *(char *) arg;
	test_t *value = (test_t *) value_ptr;
	return value->letter == letter;
}

void test_basic_operations() {
	struct deque_data deque;
	test_t *array;
	size_t idx;

	deque_init(&deque, &array, sizeof (test_t));

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

	char filter_letter = 'x', bad_letter = ' ';
	assert(deque_find(&deque, &idx, 0, matcher, &filter_letter));
	assert(filter_letter == array[idx].letter);
	assert(!deque_find(&deque, &idx, 0, matcher, &bad_letter));

	deque_free(&deque);
}

void test_move() {
	struct deque_data deque, dest_deque;
	test_t *array, *dest_array;

	deque_init(&deque, &array, sizeof (test_t));
	for (int i=0; i<100; i++) {
		size_t idx = deque_add(&deque, DEQUE_BACK);
		array[idx].num = i + 33;
		array[idx].letter = 'a' + (i % 26);
	}

	deque_move(&deque, &dest_deque, &dest_array);
	deque_remove(&dest_deque, DEQUE_FRONT);
	size_t idx = deque_add(&dest_deque, DEQUE_BACK);
	dest_array[idx].num = 133;
	dest_array[idx].letter = 'w';
	assert(100 == deque_len(&dest_deque));

	for (int i=1; i<101; i++) {
		size_t idx = deque_peek(&dest_deque, i-1, DEQUE_FRONT);
		assert(dest_array[idx].num == i + 33);
		assert(dest_array[idx].letter == 'a' + (i % 26));
	}

	deque_free(&dest_deque);
}

void test_memory() {
	struct deque_data deque;
	test_t *array;

	deque_init(&deque, &array, sizeof (test_t));
	for (int i=0; i<100; i++) {
		size_t idx = deque_add(&deque, DEQUE_BACK);
		array[idx].num = i;
		array[idx].letter = 'a' + (i % 26);
	}

	size_t before_size = array_sizeof(&deque.array_data);
	for (number_t i=0; i<10000; i++) {
		deque_remove(&deque, DEQUE_FRONT);
		deque_remove(&deque, DEQUE_FRONT);
		deque_add(&deque, DEQUE_BACK);
		deque_add(&deque, DEQUE_BACK);
	}
	size_t after_size = array_sizeof(&deque.array_data);
	assert(before_size == after_size);

	deque_free(&deque);
}

int main() {
	test_basic_operations();
	test_move();
	test_memory();
	return 0;
}
