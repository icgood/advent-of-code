#include <stdint.h>
#include <stdio.h>

typedef struct {
	int64_t part1;
	int64_t part2;
} day_result;

void day_result_check(char *dir, char *filename, int64_t want_part1, int64_t want_part2);
void day_result_print(day_result *res);

extern void day_result_compute(day_result *res, FILE *in);
extern void day_result_test(char *dir);
