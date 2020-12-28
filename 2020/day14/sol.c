#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day_base/struct/hashmap.h"

typedef char bitmask_t[36];
typedef unsigned long int number_t;

typedef struct {
	bitmask_t bitmask;
	number_t address;
	number_t value;
} assignment_t;

static number_t apply_bitmask(bitmask_t *bitmask, number_t value, char zero) {
	size_t bitmask_size = sizeof (bitmask_t);
	char *ptr = *bitmask;
	for (size_t i=0; i<bitmask_size; i++) {
		char bit = *(ptr + i);
		size_t bit_pos = bitmask_size - i - 1;
		if (bit == zero) {
			value &= ~(1UL << bit_pos);
		} else if (bit == '1') {
			value |= 1UL << bit_pos;
		}
	}
	return value;
}

static number_t sum_memory(number_t *memory, size_t memory_len) {
	number_t sum = 0;
	for (size_t i=0; i<memory_len; i++) {
		sum += memory[i];
	}
	return sum;
}

static void assign_all(bitmask_t *bitmask, number_t address, number_t value, struct hashmap_data *hashmap, size_t start) {
	size_t bitmask_size = sizeof (bitmask_t);
	char *ptr = *bitmask;
	for (size_t i=start; i<bitmask_size; i++) {
		char *bit = ptr + i;
		if (*bit == 'X') {
			*bit = '*';
			assign_all(bitmask, address, value, hashmap, i+1);
			*bit = '1';
			assign_all(bitmask, address, value, hashmap, i+1);
			*bit = 'X';
		}
	}
	address = apply_bitmask(bitmask, address, '*');
	size_t idx = hashmap_add(hashmap, &address, sizeof (number_t));
	number_t *memory = hashmap_pointer(hashmap);
	memory[idx] = value;
}

static void read(FILE *in, struct array_data *array, assignment_t **ptr) {
	char line[1024];
	bitmask_t current_bitmask;

	array_init(array, ptr, sizeof (assignment_t));
	while (fgets(line, sizeof (line), in) != NULL) {
		if (strncmp(line, "mask = ", 7) == 0) {
			memcpy(current_bitmask, line+7, sizeof (bitmask_t));
		} else {
			size_t idx = array_add(array);
			assignment_t *ptr = (assignment_t *) array_pointer(array);
			assignment_t *curr = &ptr[idx];
			memcpy(curr->bitmask, current_bitmask, sizeof (bitmask_t));
			sscanf(line, "mem[%lu] = %lu\n", &curr->address, &curr->value);
		}
	}
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	struct array_data array;
	assignment_t *assignments;

	read(in, &array, &assignments);

	struct hashmap_data hashmap;
	number_t *memory;

	// part 1
	if (strcmp(arg, "") == 0 || strcmp(arg, "part1") == 0) {
		hashmap_init(&hashmap, &memory, sizeof (number_t), 256);
		for (size_t i=0; i<array_len(&array); i++) {
			assignment_t *curr = &assignments[i];
			size_t idx = hashmap_add(&hashmap, &curr->address, sizeof (number_t));
			memory[idx] = apply_bitmask(&curr->bitmask, curr->value, '0');
		}
		res->part1 = sum_memory(memory, hashmap_len(&hashmap));
		hashmap_free(&hashmap);
	}

	// part 2
	if (strcmp(arg, "") == 0 || strcmp(arg, "part2") == 0) {
		hashmap_init(&hashmap, &memory, sizeof (number_t), 4096);
		for (size_t i=0; i<array_len(&array); i++) {
			assignment_t *curr = &assignments[i];
			assign_all(&curr->bitmask, curr->address, curr->value, &hashmap, 0);
		}
		res->part2 = sum_memory(memory, hashmap_len(&hashmap));
		hashmap_free(&hashmap);
	}

	array_free(&array);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {165, 0}, "part1"},
		{"example2", {0, 208}, "part2"},
		{DAY_INPUT, {14839536808842, 4215284199669}},
	};
	return checks;
}
