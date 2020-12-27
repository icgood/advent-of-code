#define _GNU_SOURCE

#include <stdio.h>
#include "day_base/day_result.h"

static void day_result_print(day_result *res) {
	if (res->part1 > 0) printf("part 1: %lld\n", res->part1);
	else printf("part 1: %s\n", res->part1_str);

	if (res->part2 > 0) printf("part 2: %lld\n", res->part2);
	else printf("part 2: %s\n", res->part2_str);
}

int main(int argc, char **argv) {
	char *arg = argc > 1 ? argv[1] : NULL;
	day_result res = {};
	day_result_compute(arg, &res, stdin);
	day_result_print(&res);
	return 0;
}
