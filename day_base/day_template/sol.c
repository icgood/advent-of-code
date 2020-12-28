#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {0, 0}},
		{DAY_INPUT, {0, 0}},
	};
	return checks;
}
