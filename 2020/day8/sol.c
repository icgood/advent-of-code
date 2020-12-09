#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day_base/struct/bitmap.h"

typedef enum {
	ACC,
	JMP,
	NOP,
} operation;

typedef struct {
	operation op;
	int val;
} instruction;

static void flip_op(instruction *i) {
	if (JMP == i->op) i->op = NOP;
	else if (NOP == i->op) i->op = JMP;
}

static size_t run(struct array_data *data, instruction *array, int *accumulator) {
	size_t pos = 0, len = array_len(data);
	struct bitmap_data bitmap;
	bitmap_init(&bitmap, len);
	do {
		bitmap_set(&bitmap, pos);
		if (ACC == array[pos].op) {
			*accumulator += array[pos].val;
			pos++;
		} else if (JMP == array[pos].op) {
			pos += array[pos].val;
		} else {
			pos++;
		}
	} while (bitmap_get(&bitmap, pos) == 0 && pos != len);
	bitmap_free(&bitmap);
	return pos;
}

void day_result_compute(char *arg, day_result *res, FILE *in) {
	char line[1024];
	struct array_data data;
	instruction *array;

	array_init(&data, &array, sizeof (instruction));
	while (fgets(line, sizeof (line), in) != NULL) {
		size_t idx = array_add(&data);

		if (strncmp(line, "acc", 3) == 0) array[idx].op = ACC;
		else if (strncmp(line, "jmp", 3) == 0) array[idx].op = JMP;
		else array[idx].op = NOP;
		array[idx].val = atoi(line + 4);
	}

	// part 1
	{
		int accumulator = 0;
		run(&data, array, &accumulator);
		res->part1 = accumulator;
	}

	// part 2
	size_t len = array_len(&data);
	for (size_t i=0; i<len; i++) {
		int accumulator = 0;
		if (JMP == array[i].op || NOP == array[i].op) {
			flip_op(&array[i]);
			size_t end = run(&data, array, &accumulator);
			if (end == len) {
				res->part2 = accumulator;
				break;
			}
			flip_op(&array[i]);
		}
	}

	array_free(&data);
}

void day_answers_provide(day_arguments *args, day_answers *answers) {
	*answers = (day_answers) {
		{5, 8},
		{1859, 1235},
	};
}
