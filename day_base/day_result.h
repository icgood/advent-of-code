#include <stdio.h>

typedef long long int day_result_num_t;

typedef struct {
	day_result_num_t part1;
	day_result_num_t part2;
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
