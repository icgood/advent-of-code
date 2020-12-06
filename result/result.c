#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "result.h"

void print_result(result *res) {
	printf("part 1: %lld\n", res->part1);
	printf("part 2: %lld\n", res->part2);
}

void test_result(char *filename, int64_t check_part1, int64_t check_part2) {
	result res = {};
	FILE *in = fopen(filename, "r");
	assert(in != NULL);
	compute_result(&res, in);
	assert(res.part1 == check_part1);
	assert(res.part2 == check_part2);
	fclose(in);
}

int main() {
	result res = {};
	compute_result(&res, stdin);
	printf("part 1: %lld\n", res.part1);
	printf("part 2: %lld\n", res.part2);
	return 0;
}
