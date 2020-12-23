#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

void day_result_compute(char *arg, day_result *res, FILE *in) {
}

void day_answers_provide(day_arguments *args, day_answers *answers) {
	*answers = (day_answers) {
		{0, 0},
		{0, 0},
	};
}
