#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

void day_result_compute(day_result *res, FILE *in) {
	uint8_t bitmap[256];
	char line[1024];
	memset(bitmap, 0, sizeof (bitmap));

	while (fgets(line, sizeof (line), in) != NULL) {
		uint32_t value;
		sscanf(line, "%u", &value);
		bitmap[value / 8] |= (1 << (value % 8));
	}

	// part 1
	for (uint32_t i=0; i<2020; i++) {
		uint32_t j = 2020 - i;
		if ((bitmap[i / 8] & (1 << (i % 8))) != 0 && (bitmap[j / 8] & (1 << (j % 8))) != 0) {
			res->part1 = i * j;
		}
	}

	// part 2
	for (uint32_t i=0; i<2020; i++) {
		for (uint32_t j=i+1; j<2020-i; j++) {
			uint32_t k = 2020 - i - j;
			if ((bitmap[i / 8] & (1 << (i % 8))) != 0 
					&& (bitmap[j / 8] & (1 << (j % 8))) != 0
					&& (bitmap[k / 8] & (1 << (k % 8))) != 0) {
				res->part2 = i * j * k;
			}
		}
	}
}

void day_result_test(char *dir) {
	day_result_check(dir, "example", 514579, 241861950);
	day_result_check(dir, "input", 270144, 261342720);
}
