#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/deque.h"

typedef unsigned long int number_t;
typedef unsigned long 	int label_t;

static int label_matcher(deque_t *data, size_t node, const void *value_ptr, void *arg_ptr) {
	const label_t *wanted_label = (label_t *) arg_ptr;
	const label_t *label = (const label_t *) value_ptr;
	return memcmp(label, wanted_label, sizeof (label_t)) == 0;
}

static void load_cups(deque_t *deque, label_t **cups, label_t *starting_labels, size_t num_starting, size_t num_total) {
	for (size_t i=0; i<num_starting; i++) {
		size_t idx = deque_add(deque, DEQUE_BACK);
		(*cups)[idx] = starting_labels[i];
	}
	for (size_t i=num_starting; i<num_total; i++) {
		size_t idx = deque_add(deque, DEQUE_BACK);
		(*cups)[idx] = i+1;
	}
}

static size_t find_node(size_t label, size_t *starting_pos, size_t num_starting) {
	if (label <= num_starting) {
		return starting_pos[label];
	} else {
		return label - 1;
	}
}

static void move_cups(deque_t *deque, label_t *cups, size_t *starting_pos, size_t num_starting) {
	label_t removed[3];
	label_t current, destination;
	{
		size_t idx = deque_peek(deque, 0, DEQUE_FRONT);
		current = cups[idx];
	}
	deque_rotate(deque, 1, DEQUE_LEFT);
	for (size_t i=0; i<3; i++) {
		size_t idx = deque_peek(deque, i, DEQUE_FRONT);
		removed[i] = cups[idx];
	}
	for (label_t label=current-1; label!=current; label--) {
		if (label == 0) label = deque_len(deque);
		if (removed[0] != label && removed[1] != label && removed[2] != label) {
			destination = label;
			break;
		}
	}
	size_t destination_pos = find_node(destination, starting_pos, num_starting);
	size_t head = deque_head(deque);
	deque_set_head(deque, destination_pos);
	deque_rotate(deque, 1, DEQUE_LEFT);
	size_t shift_pos = deque_head(deque);
	deque_set_head(deque, head);
	deque_shift(deque, 3, shift_pos);
}

static number_t get_res_part1(deque_t *deque, label_t *cups) {
	size_t destination = 1;
	deque_find(deque, NULL, DEQUE_LEFT, DEQUE_ROTATE, label_matcher, &destination);
	size_t len = deque_len(deque);
	char out[len];
	for (size_t i=1; i<len; i++) {
		size_t idx = deque_peek(deque, i, DEQUE_FRONT);
		out[i-1] = '0' + cups[idx];
	}
	out[len-1] = '\0';
	return atol(out);
}

static number_t get_res_part2(deque_t *deque, label_t *cups) {
	size_t destination = 1;
	deque_find(deque, NULL, DEQUE_LEFT, DEQUE_ROTATE, label_matcher, &destination);
	size_t idx_one = deque_peek(deque, 1, DEQUE_FRONT);
	size_t idx_two = deque_peek(deque, 2, DEQUE_FRONT);
	return cups[idx_one] * cups[idx_two];
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	label_t starting_labels[32];
	size_t starting_pos[32], num_starting;
	char letter;

	for (num_starting=0; (letter = fgetc(in)) != '\n'; num_starting++) {
		starting_pos[letter - '0'] = num_starting;
		starting_labels[num_starting] = letter - '0';
	}
	deque_t deque;
	label_t *cups;

	// part 1
	deque_init(&deque, &cups, sizeof (label_t));
	load_cups(&deque, &cups, starting_labels, num_starting, num_starting);
	for (size_t i=0; i<100; i++) {
		move_cups(&deque, cups, starting_pos, num_starting);
	}
	res->part1 = get_res_part1(&deque, cups);
	deque_free(&deque);

	// part 2
	deque_init(&deque, &cups, sizeof (label_t));
	load_cups(&deque, &cups, starting_labels, num_starting, 1000000);
	for (size_t i=0; i<10000000; i++) {
		move_cups(&deque, cups, starting_pos, num_starting);
	}
	res->part2 = get_res_part2(&deque, cups);
	deque_free(&deque);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {67384529, 149245887792}},
		{DAY_INPUT, {49725386, 538935646702}},
	};
	return checks;
}
