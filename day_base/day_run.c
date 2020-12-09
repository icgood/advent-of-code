#include <stdio.h>
#include "day_base/day_result.h"

static void day_result_print(day_result *res) {
	printf("part 1: %lld\n", res->part1);
	printf("part 2: %lld\n", res->part2);
}

int main(int argc, char **argv) {
	char *arg = argc > 1 ? argv[1] : NULL;
	day_result res = {};
	day_result_compute(arg, &res, stdin);
	day_result_print(&res);
	return 0;
}
