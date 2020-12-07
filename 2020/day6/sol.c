#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

static uint32_t *new_array(size_t new_size, uint32_t *old_array, size_t len, int val) {
	uint32_t *new_array = malloc(new_size * sizeof (uint32_t));
	memset(new_array, val, new_size * sizeof (uint32_t));
	if (len > 0) memcpy(new_array, old_array, len * sizeof (uint32_t));
	free(old_array);
	return new_array;
}

static int count_bits(uint32_t val) {
	int count = 0;
	for (int i=0; i<32; i++) {
		if ((val & (1 << i)) != 0) count++;
	}
	return count;
}

void day_result_compute(day_result *res, FILE *in) {
	char line[32];
	size_t len = 0, size = 2;
	uint32_t *array1 = new_array(size, NULL, 0, 0);
	uint32_t *array2 = new_array(size, NULL, 0, 255);

	while (fgets(line, sizeof (line), in) != NULL) {
		size_t line_len = strspn(line, "abcdefghijklmnopqrstuvwxyz");
		if (line_len == 0) {
			len++;
			if (len >= size) {
				size += 2;
				array1 = new_array(size, array1, len, 0);
				array2 = new_array(size, array2, len, 255);
			}
			continue;
		}
		uint32_t line_bits = 0;
		for (size_t i=0; i<line_len; i++) {
			line_bits |= (1 << (line[i] - 'a'));
		}
		array1[len] |= line_bits;
		array2[len] &= line_bits;
	}
	len++;

	// part 1
	for (size_t i=0; i<len; i++) {
		res->part1 += count_bits(array1[i]);
	}

	// part 2
	for (size_t i=0; i<len; i++) {
		res->part2 += count_bits(array2[i]);
	}

	free(array1);
	free(array2);
}

void day_answers_provide(day_answers *answers) {
	*answers = (day_answers) {
		{11, 6},
		{7283, 3520},
	};
}
