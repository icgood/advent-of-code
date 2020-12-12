#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day12.h"

typedef struct {
	action_t action;
	unsigned int value;
} instruction_t;

extern apply_funcs_t part1_functions;
extern apply_funcs_t part2_functions;

void day_result_compute(char *arg, day_result *res, FILE *in) {
	struct array_data array;
	instruction_t *instructions;
	array_init(&array, &instructions, sizeof (instruction_t));

	char action_letter;
	unsigned int value;
	while (fscanf(in, "%c%u\n", &action_letter, &value) > 0) {
		size_t idx = array_add(&array);
		instruction_t *ins = &instructions[idx];
		if (action_letter == 'N') ins->action = NORTH;
		else if (action_letter == 'S') ins->action = SOUTH;
		else if (action_letter == 'E') ins->action = EAST;
		else if (action_letter == 'W') ins->action = WEST;
		else if (action_letter == 'L') ins->action = LEFT;
		else if (action_letter == 'R') ins->action = RIGHT;
		else if (action_letter == 'F') ins->action = FORWARD;
		ins->value = value;
	}

	// part 1
	state_t state1 = {};
	for (size_t i=0; i<array_len(&array); i++) {
		apply_t apply = part1_functions[instructions[i].action];
		apply(instructions[i].value, &state1);
	}
	res->part1 = abs(state1.ship_north_south) + abs(state1.ship_east_west);

	// part 2
	state_t state2 = {0, 0, 0, 1, 10};
	for (size_t i=0; i<array_len(&array); i++) {
		apply_t apply = part2_functions[instructions[i].action];
		apply(instructions[i].value, &state2);
	}
	res->part2 = abs(state2.ship_north_south) + abs(state2.ship_east_west);

	array_free(&array);
}

void day_answers_provide(day_arguments *args, day_answers *answers) {
	*answers = (day_answers) {
		{25, 286},
		{319, 50157},
	};
}
