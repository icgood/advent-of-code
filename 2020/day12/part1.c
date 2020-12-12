#include "day12.h"

static void move_north(unsigned int value, state_t *state) {
	state->ship_north_south += value;
}

static void move_south(unsigned int value, state_t *state) {
	state->ship_north_south -= value;
}

static void move_east(unsigned int value, state_t *state) {
	state->ship_east_west += value;
}

static void move_west(unsigned int value, state_t *state) {
	state->ship_east_west -= value;
}

static void turn_left(unsigned int value, state_t *state) {
	state->direction += value;
	if (state->direction >= 360) state->direction -= 360;
}

static void turn_right(unsigned int value, state_t *state) {
	if (value > state->direction) state->direction += 360;
	state->direction -= value;
}

static void move_forward(unsigned int value, state_t *state) {
	if (state->direction == 0) move_east(value, state);
	else if (state->direction == 90) move_north(value, state);
	else if (state->direction == 180) move_west(value, state);
	else if (state->direction == 270) move_south(value, state);
}

apply_funcs_t part1_functions = {
	move_north,
	move_south,
	move_east,
	move_west,
	turn_left,
	turn_right,
	move_forward,
};
