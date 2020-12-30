#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include "day_base/day_result.h"

static void day_result_print(day_result_t *res) {
	if (strlen(res->part1_str) > 0) printf("part 1: %s\n", res->part1_str);
	else printf("part 1: %lld\n", res->part1);

	if (strlen(res->part2_str) > 0) printf("part 2: %s\n", res->part2_str);
	else printf("part 2: %lld\n", res->part2);
}

int main(int argc, char **argv) {
	char *arg = argc > 1 ? argv[1] : NULL;
	day_result_t res = {};
	day_result_compute(arg, &res, stdin);
	day_result_print(&res);
	return 0;
}
