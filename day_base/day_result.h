#include <stdint.h>
#include <stdio.h>

typedef struct {
	int64_t part1;
	int64_t part2;
} day_result;

extern void day_result_compute(day_result *res, FILE *in);

typedef struct {
	day_result example;
	day_result input;
} day_answers;

extern void day_answers_provide(day_answers *answers);
