#define _GNU_SOURCE

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

static void day_result_check(char *dir, char *filename, char *arg, day_result_t *want) {
	char path[PATH_MAX] = "";
	strcat(path, dir);
	strcat(path, "/");
	strcat(path, filename);

	day_result_t res = {};
	FILE *in = fopen(path, "r");
	assert(in != NULL);
	day_result_compute(arg, &res, in);
	fclose(in);

	if (strlen(want->part1_str) > 0) {
		if (strcmp(want->part1_str, res.part1_str) != 0) {
			fprintf(stderr, "%s part1\ngot:  %s\nwant: %s\n", filename, res.part1_str, want->part1_str);
			exit(1);
		}
	} else {
		if (want->part1 != res.part1) {
			fprintf(stderr, "%s part1\ngot:  %lld\nwant: %lld\n", filename, res.part1, want->part1);
			exit(1);
		}
	}

	if (strlen(want->part2_str) > 0) {
		if (strcmp(want->part2_str, res.part2_str) != 0) {
			fprintf(stderr, "%s part2\ngot:  %s\nwant: %s\n", filename, res.part2_str, want->part2_str);
			exit(1);
		}
	} else {
		if (want->part2 != res.part2) {
			fprintf(stderr, "%s part2\ngot:  %lld\nwant: %lld\n", filename, res.part2, want->part2);
			exit(1);
		}
	}
}

int main(int argc, char **argv) {
	assert(argc > 2);
	char *dir = argv[1];

	day_check_t *checks = day_check_provide();

	for (int i=2; i<argc; i++) {
		char *file = argv[i];
		day_check_t *check = &checks[i-2];
		assert(strcmp(file, check->file) == 0);
		day_result_check(dir, check->file, check->arg, &check->result);
	}

	return 0;
}
