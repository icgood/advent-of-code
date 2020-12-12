#include "day12.h"

static void move_north(unsigned int value, state_t *state) {
	state->wp_north_south += value;
}

static void move_south(unsigned int value, state_t *state) {
	state->wp_north_south -= value;
}

static void move_east(unsigned int value, state_t *state) {
	state->wp_east_west += value;
}

static void move_west(unsigned int value, state_t *state) {
	state->wp_east_west -= value;
}

static void turn_left(unsigned int value, state_t *state) {
	while (value > 0) {
		int wp_north_south = state->wp_north_south;
		state->wp_north_south = state->wp_east_west;
		state->wp_east_west = -wp_north_south;
		value -= 90;
	}
}

static void turn_right(unsigned int value, state_t *state) {
	while (value > 0) {
		int wp_north_south = state->wp_north_south;
		state->wp_north_south = -state->wp_east_west;
		state->wp_east_west = wp_north_south;
		value -= 90;
	}
}

static void move_forward(unsigned int value, state_t *state) {
	state->ship_north_south += state->wp_north_south * value;
	state->ship_east_west += state->wp_east_west * value;
}

apply_funcs_t part2_functions = {
	move_north,
	move_south,
	move_east,
	move_west,
	turn_left,
	turn_right,
	move_forward,
};
