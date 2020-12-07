#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

void day_result_print(day_result *res) {
	printf("part 1: %lld\n", res->part1);
	printf("part 2: %lld\n", res->part2);
}

void day_result_check(char *dir, char *filename, int64_t want_part1, int64_t want_part2) {
	char path[PATH_MAX] = "";
	strcat(path, dir);
	strcat(path, "/");
	strcat(path, filename);

	day_result res = {};
	FILE *in = fopen(path, "r");
	assert(in != NULL);
	day_result_compute(&res, in);
	fclose(in);

	if (want_part1 != res.part1) {
		fprintf(stderr, "%s part1 got:%lld want:%lld\n", filename, res.part1, want_part1);
		exit(1);
	}
	if (want_part2 != res.part2) {
		fprintf(stderr, "%s part2 got:%lld want:%lld\n", filename, res.part2, want_part2);
		exit(1);
	}
}
