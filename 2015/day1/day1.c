#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "result/result.h"

void compute_result(result *res, FILE *in) {
	char line[102400];
	fgets(line, sizeof (line), in);
	size_t len = strspn(line, "()");

	// part 1
	for (size_t i=0; i<len; i++) {
		if (line[i] == '(') res->part1++;
		else res->part1--;
	}

	// part 2
	int64_t floor = 0;
	for (size_t i=0; i<len; i++) {
		if (line[i] == '(') floor++;
		else floor--;
		if (floor == -1) {
			res->part2 = i+1;
			break;
		}
	}
}
