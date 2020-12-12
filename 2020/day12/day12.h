#ifndef FILE_DAY12_TYPES_H
#define FILE_DAY12_TYPES_H

typedef enum { NORTH, SOUTH, EAST, WEST, LEFT, RIGHT, FORWARD } action_t;

typedef struct {
	unsigned int direction;
	int ship_north_south;
	int ship_east_west;
	int wp_north_south;
	int wp_east_west;
} state_t;

typedef void apply_t(unsigned int value, state_t *state);

typedef apply_t *apply_funcs_t[7];

#endif
