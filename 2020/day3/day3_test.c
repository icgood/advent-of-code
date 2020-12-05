#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "day3.h"

int main() {
	day3_result ex_res = {}, in_res = {};

	FILE *ex_file = fopen("2020/day3/example", "r");
	assert(ex_file != NULL);
	day3(&ex_res, ex_file);
	fclose(ex_file);

	assert(7 == ex_res.part1);
	assert(336 == ex_res.part2);

	FILE *in_file = fopen("2020/day3/input", "r");
	assert(in_file != NULL);
	day3(&in_res, in_file);
	fclose(in_file);

	assert(216 == in_res.part1);
	assert(6708199680 == in_res.part2);

	return 0;
}
