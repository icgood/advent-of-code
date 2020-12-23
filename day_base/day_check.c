#define _GNU_SOURCE

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

static void day_result_check(char *dir, char *filename, char *arg, day_result *want) {
	char path[PATH_MAX] = "";
	strcat(path, dir);
	strcat(path, "/");
	strcat(path, filename);

	day_result res = {};
	FILE *in = fopen(path, "r");
	assert(in != NULL);
	day_result_compute(arg, &res, in);
	fclose(in);

	if (want->part1 != res.part1) {
		fprintf(stderr, "%s part1 got:%lld want:%lld\n", filename, res.part1, want->part1);
		exit(1);
	}
	if (want->part2 != res.part2) {
		fprintf(stderr, "%s part2 got:%lld want:%lld\n", filename, res.part2, want->part2);
		exit(1);
	}
}

int main(int argc, char **argv) {
	assert(argc >= 2);
	char *dir = argv[1];

	day_arguments args = {};
	day_answers answers = {};
	day_answers_provide(&args, &answers);

	day_result_check(dir, "example", args.example_arg, &answers.example);
	day_result_check(dir, "input", args.input_arg, &answers.input);

	return 0;
}
