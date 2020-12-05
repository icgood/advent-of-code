#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "day4.h"

int main() {
	day4_result ex_res = {}, in_res = {};

	FILE *ex_file = fopen("2020/day4/example", "r");
	assert(ex_file != NULL);
	day4(&ex_res, ex_file);
	fclose(ex_file);

	assert(2 == ex_res.part1);
	assert(2 == ex_res.part2);

	FILE *in_file = fopen("2020/day4/input", "r");
	assert(in_file != NULL);
	day4(&in_res, in_file);
	fclose(in_file);

	assert(192 == in_res.part1);
	assert(101 == in_res.part2);

	return 0;
}
