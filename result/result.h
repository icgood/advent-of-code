#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int64_t part1;
	int64_t part2;
} result;

void compute_result(result *res, FILE *in);
void print_result(result *res);
void test_result(char *filename, int64_t check_part1, int64_t check_part2);
