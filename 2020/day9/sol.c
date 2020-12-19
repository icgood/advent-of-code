#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day_base/struct/deque.h"

typedef unsigned long long int number;

static int is_sum(number num, number *window, size_t window_len) {
	for (size_t i=0; i<window_len; i++) {
		for (size_t j=0; j<i; j++) {
			if (window[i] + window[j] == num) {
				return 1;
			}
		}
	}
	return 0;
}

static number compute_part2(struct deque_data *deque, number *array) {
	size_t zero_idx = deque_peek(deque, 0, DEQUE_FRONT);
	number min = array[zero_idx], max = array[zero_idx];
	for (size_t i=1; i<deque_len(deque); i++) {
		size_t idx = deque_peek(deque, i, DEQUE_FRONT);
		if (array[idx] < min) min = array[idx];
		if (array[idx] > max) max = array[idx];
	}
	printf("%llu %llu\n", min, max);
	return min + max;
}

void day_result_compute(char *arg, day_result *res, FILE *in) {
	const size_t window_len = arg != NULL ? atoi(arg) : 0;
	assert(window_len > 0);
	struct array_data data;
	number *array;
	number num;

	array_init(&data, &array, sizeof (number));
	while (fscanf(in, "%llu\n", &num) > 0) {
		size_t idx = array_add(&data);
		array[idx] = num;
	}

	// part 1
	number window[window_len];
	size_t window_start = 0;
	for (size_t i=0; i<array_len(&data); i++) {
		num = array[i];
		if (i >= window_len && !is_sum(num, window, window_len)) {
			res->part1 = num;
			break;
		}
		window[window_start++] = num;
		if (window_start >= window_len) window_start = 0;
	}

	// part 2
	number goal = res->part1;
	struct deque_data deque;
	number *contiguous;
	deque_init(&deque, &contiguous, sizeof (number));
	for (size_t i=0; i<array_len(&data); i++) {
		size_t idx = deque_add(&deque, DEQUE_BACK);
		contiguous[idx] = array[i];
		number sum = 0;
		for (size_t j=0; j<deque_len(&deque); j++) {
			size_t c_idx = deque_peek(&deque, j, DEQUE_FRONT);
			sum += contiguous[c_idx];
		}
		while (sum > goal) {
			size_t c_idx = deque_remove(&deque, DEQUE_FRONT);
			sum -= contiguous[c_idx];
		}
		if (sum == goal) {
			res->part2 = compute_part2(&deque, contiguous);
			break;
		}
	}
	deque_free(&deque);

	array_free(&data);
}

void day_answers_provide(day_arguments *args, day_answers *answers) {
	*args = (day_arguments) {
		"5",
		"25",
	};
	*answers = (day_answers) {
		{127, 62},
		{400480901, 67587168},
	};
}
