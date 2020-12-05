#include <stdio.h>
#include <stdlib.h>

typedef struct {
	uint64_t part1;
	uint64_t part2;
} result;

void compute_result(result *res, FILE *in);
void print_result(result *res);
void test_result(char *filename, uint64_t check_part1, uint64_t check_part2);
