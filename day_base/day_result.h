#include <stdio.h>

#define DAY_INPUT "input"

typedef long long int day_result_num_t;
typedef char day_result_str_t[256];

typedef struct {
	day_result_num_t part1;
	day_result_num_t part2;
	day_result_str_t part1_str;
	day_result_str_t part2_str;
} day_result_t;

extern void day_result_compute(char *arg, day_result_t *res, FILE *in);

typedef struct {
	char file[128];
	day_result_t result;
	char arg[128];
} day_check_t;

extern day_check_t *day_check_provide();
