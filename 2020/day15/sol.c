#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

typedef unsigned int number_t;

static number_t get_spoken(number_t *starting, size_t starting_len, size_t n) {
	number_t *cache = malloc(sizeof (number_t) * n);
	for (size_t i=0; i<n; i++) cache[i] = 0;
	for (size_t i=1; i<starting_len; i++) {
		number_t num = starting[i-1];
		cache[num] = i;
	}
	number_t prev = 0, recent = starting[starting_len-1];
	for (size_t i=starting_len; i<=n; i++) {
		prev = recent;
		number_t prev_i = cache[prev];
		recent = prev_i != 0 ? (i - prev_i) : 0;
		cache[prev] = i;
	}
	free(cache);
	return prev;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	char line[1024];
	size_t starting_len = 0;
	number_t starting[100];

	assert(fgets(line, sizeof (line), in) != NULL);
	char *ptr = line;
	while (*ptr != '\n' && *ptr != '\0') {
		starting[starting_len++] = atoi(ptr);
		ptr += strspn(ptr, "0123456789");
		ptr += strspn(ptr, ",");
	}

	// part 1
	res->part1 = get_spoken(starting, starting_len, 2020);

	// part 2
	res->part2 = get_spoken(starting, starting_len, 30000000);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {436, 175594}},
		{DAY_INPUT, {662, 37312}},
	};
	return checks;
}
