#include <assert.h>
#include <stdio.h>
#include "deque.h"

typedef unsigned long int number_t;

typedef struct {
	number_t num;
	char letter;
} test_t;

int node_matcher(struct deque_data *data, size_t node, const void *value_ptr, void *arg_ptr) {
	size_t *node_ptr = (size_t *) arg_ptr;
	return node == *node_ptr;
}

int letter_matcher(struct deque_data *data, size_t node, const void *value_ptr, void *arg_ptr) {
	char letter = *(char *) arg_ptr;
	test_t *value = (test_t *) value_ptr;
	return value->letter == letter;
}

void test_basic_operations() {
	struct deque_data deque;
	test_t *array;
	size_t idx;

	deque_init(&deque, &array, sizeof (test_t));
	idx = deque_add(&deque, DEQUE_FRONT);
	assert(idx == deque_peek(&deque, 0, DEQUE_FRONT));
	assert(idx == deque_peek(&deque, 0, DEQUE_BACK));
	size_t idx2 = deque_add(&deque, DEQUE_FRONT);
	assert(idx2 == deque_peek(&deque, 0, DEQUE_FRONT));
	assert(idx == deque_peek(&deque, 1, DEQUE_FRONT));
	assert(idx == deque_peek(&deque, 0, DEQUE_BACK));
	assert(idx2 == deque_peek(&deque, 1, DEQUE_BACK));
	size_t idx3 = deque_add(&deque, DEQUE_BACK);
	assert(idx2 == deque_peek(&deque, 0, DEQUE_FRONT));
	assert(idx == deque_peek(&deque, 1, DEQUE_FRONT));
	assert(idx3 == deque_peek(&deque, 2, DEQUE_FRONT));
	assert(idx3 == deque_peek(&deque, 0, DEQUE_BACK));
	assert(idx == deque_peek(&deque, 1, DEQUE_BACK));
	assert(idx2 == deque_peek(&deque, 2, DEQUE_BACK));
	deque_free(&deque);

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

	size_t before_size = array_sizeof(&deque.node_array) + array_sizeof(&deque.value_array);
	for (number_t i=0; i<10000; i++) {
		deque_remove(&deque, DEQUE_FRONT);
		deque_remove(&deque, DEQUE_FRONT);
		deque_add(&deque, DEQUE_BACK);
		deque_add(&deque, DEQUE_BACK);
	}
	size_t after_size = array_sizeof(&deque.node_array) + array_sizeof(&deque.value_array);
	assert(before_size == after_size);

	deque_free(&deque);
}

void test_rotate_shift() {
	struct deque_data deque;
	test_t *array;
	size_t idx;

	deque_init(&deque, &array, sizeof (test_t));
	for (number_t i=0; i<100; i++) {
		idx = deque_add(&deque, DEQUE_BACK);
		array[idx].num = i;
		array[idx].letter = 'a' + (i % 26);
	}

	idx = deque_peek(&deque, 0, DEQUE_FRONT);
	assert(0 == array[idx].num);
	assert('a' == array[idx].letter);

	char letter = 'd';
	size_t head = deque_head(&deque);

	deque_find(&deque, &idx, DEQUE_LEFT, DEQUE_ROTATE, letter_matcher, &letter);
	assert(3 == array[idx].num);
	assert(letter == array[idx].letter);
	deque_shift(&deque, 1, head);
	deque_find(&deque, &idx, DEQUE_LEFT, DEQUE_ROTATE, letter_matcher, &letter);
	assert(29 == array[idx].num);
	assert(letter == array[idx].letter);
	deque_shift(&deque, 1, head);
	deque_find(&deque, &idx, DEQUE_LEFT, DEQUE_ROTATE, letter_matcher, &letter);
	assert(55 == array[idx].num);
	assert(letter == array[idx].letter);
	deque_shift(&deque, 1, head);
	deque_find(&deque, &idx, DEQUE_LEFT, DEQUE_ROTATE, letter_matcher, &letter);
	assert(81 == array[idx].num);
	assert(letter == array[idx].letter);
	deque_shift(&deque, 1, head);
	deque_find(&deque, NULL, DEQUE_LEFT, DEQUE_ROTATE, node_matcher, &head);

	deque_set_head(&deque, head);
	idx = deque_peek(&deque, 0, DEQUE_FRONT);
	assert(0 == array[idx].num);
	assert('a' == array[idx].letter);
	idx = deque_peek(&deque, 3, DEQUE_BACK);
	assert(3 == array[idx].num);
	assert(letter == array[idx].letter);
	idx = deque_peek(&deque, 2, DEQUE_BACK);
	assert(29 == array[idx].num);
	assert(letter == array[idx].letter);
	idx = deque_peek(&deque, 1, DEQUE_BACK);
	assert(55 == array[idx].num);
	assert(letter == array[idx].letter);
	idx = deque_peek(&deque, 0, DEQUE_BACK);
	assert(81 == array[idx].num);
	assert(letter == array[idx].letter);

	deque_free(&deque);
}

int main() {
	test_basic_operations();
	test_move();
	test_memory();
	test_rotate_shift();
	return 0;
}
