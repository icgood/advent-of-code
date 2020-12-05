#include <stdio.h>
#include "day4.h"

int main() {
	day4_result res = {};
	day4(&res, stdin);
	printf("part 1: %lld\n", res.part1);
	printf("part 2: %lld\n", res.part2);
	return 0;
}
