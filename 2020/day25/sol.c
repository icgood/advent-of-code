#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

typedef unsigned long int number_t;

const number_t PRIME = 20201227;

static number_t find_loop_size(number_t subject_num, number_t public_key) {
	number_t value = 1, n;
	for (n=0; value != public_key; n++) {
		value *= subject_num;
		value %= PRIME;
	}
	return n;
}

static number_t transform_subject_num(number_t public_key, number_t loop_size) {
	number_t value = 1;
	for (number_t n=0; n<loop_size; n++) {
		value *= public_key;
		value %= PRIME;
	}
	return value;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	char line[256];
	number_t public_keys[2];

	assert(fgets(line, sizeof (line), in) != NULL);
	public_keys[0] = atol(line);
	assert(fgets(line, sizeof (line), in) != NULL);
	public_keys[1] = atol(line);

	// part 1
	number_t loop_sizes[2] = {
		find_loop_size(7, public_keys[0]),
		find_loop_size(7, public_keys[1]),
	};
	res->part1 = transform_subject_num(public_keys[0], loop_sizes[1]);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {14897079, 0}},
		{DAY_INPUT, {448851, 0}},
	};
	return checks;
}
