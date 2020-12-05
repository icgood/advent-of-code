#include <stdio.h>
#include "day3.h"

int main() {
	day3_result res = {};
	day3(&res, stdin);
	printf("part 1: %lld\n", res.part1);
	printf("part 2: %lld\n", res.part2);
	return 0;
}
