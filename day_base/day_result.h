#include <stdio.h>

typedef struct {
	long long int part1;
	long long int part2;
} day_result;

extern void day_result_compute(char *arg, day_result *res, FILE *in);

typedef struct {
	char *example_arg;
	char *input_arg;
} day_arguments;

typedef struct {
	day_result example;
	day_result input;
} day_answers;

extern void day_answers_provide(day_arguments *args, day_answers *answers);
