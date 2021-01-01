#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day_base/struct/bitmap.h"
#include "day_base/struct/hashmap.h"

typedef enum { WHITE, WHITE2, BLACK, BLACK2 } color_t;
typedef enum { END, EAST, SOUTHEAST, SOUTHWEST, WEST, NORTHWEST, NORTHEAST } dir_t;
const char *DIR_NAMES[7] = {NULL, "e", "se", "sw", "w", "nw", "ne"};
typedef dir_t tile_t[32];
typedef long int dim_t;

typedef struct {
	dim_t northeast;
	dim_t east;
} coord_t;

typedef struct {
	color_t color;
	bitmap_t bitmap;
	unsigned char buffer[13];
} status_t;

static coord_t add_dir(coord_t coord, dir_t dir) {
	if (dir == EAST) coord.east++;
	else if (dir == WEST) coord.east--;
	else if (dir == NORTHEAST) coord.northeast++;
	else if (dir == SOUTHWEST) coord.northeast--;
	else if (dir == NORTHWEST) {
		coord.east--;
		coord.northeast++;
	} else if (dir == SOUTHEAST) {
		coord.east++;
		coord.northeast--;
	}
	return coord;
}

static void check_tile(hashmap_t *hashmap, coord_t *coord, status_t *status) {
	size_t flipped = 0;
	status_t *statuses = hashmap_pointer(hashmap);
	for (dir_t dir=1; dir<=6; dir++) {
		coord_t dir_coord = add_dir(*coord, dir);
		size_t *idx = hashmap_lookup(hashmap, &dir_coord, sizeof (coord_t));
		if (idx != NULL) {
			status_t *dir_status =  &statuses[*idx];
			if (dir_status->color == BLACK || dir_status->color == WHITE2) flipped++;
		}
	}
	if (status->color == BLACK && (flipped == 0 || flipped > 2)) {
		status->color = WHITE2;
	} else if (status->color == WHITE && flipped == 2) {
		status->color = BLACK2;
	}
}

static void run_round(struct hashmap_key *key, void *value_ptr, size_t idx, void *arg_ptr) {
	hashmap_t *hashmap = key->hashmap;
	coord_t *coord = (coord_t *) key->buf;
	status_t *status = (status_t *) value_ptr;
	size_t round = *(size_t *) arg_ptr;
	if (bitmap_get(&status->bitmap, round)) return;
	bitmap_set(&status->bitmap, round);
	check_tile(hashmap, coord, status);
	for (dir_t dir=1; dir<=6; dir++) {
		coord_t dir_coord = add_dir(*coord, dir);
		size_t *idx = hashmap_lookup(hashmap, &dir_coord, sizeof (coord_t));
		if (idx == NULL) {
			status_t dir_status = {};
			check_tile(hashmap, &dir_coord, &dir_status);
			if (dir_status.color != WHITE) {
				size_t idx = hashmap_add(hashmap, &dir_coord, sizeof (coord_t));
				status_t *statuses = hashmap_pointer(hashmap);
				status_t *new_status = &statuses[idx];
				bitmap_init_static(&new_status->bitmap, 13);
				bitmap_set(&new_status->bitmap, round);
				new_status->color = dir_status.color;
			}
		}
	}
}

static void finish_round(struct hashmap_key *key, void *value_ptr, size_t idx, void *arg_ptr) {
	status_t *status = (status_t *) value_ptr;
	if (status->color == WHITE2) status->color = WHITE;
	if (status->color == BLACK2) status->color = BLACK;
}

static void count_flipped(struct hashmap_key *key, void *value_ptr, size_t idx, void *arg_ptr) {
	status_t *status = (status_t *) value_ptr;
	day_result_num_t *res = (day_result_num_t *) arg_ptr;
	if (status->color == BLACK) (*res)++;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	char line[128];
	array_t array;
	tile_t *tiles;

	array_init(&array, &tiles, sizeof (tile_t));
	while (fgets(line, sizeof (line), in) != NULL) {
		size_t idx = array_add(&array);
		tile_t *tile = &tiles[idx];
		char *ptr = line;
		size_t dir_n;
		for (dir_n=0; *ptr != '\n' && *ptr != '\0'; dir_n++) {
			dir_t dir = END;
			for (size_t i=1; i<7; i++) {
				const char *dir_name = DIR_NAMES[i];
				size_t dir_name_len = strlen(dir_name);
				if (strncmp(ptr, dir_name, dir_name_len) == 0) {
					ptr += dir_name_len;
					dir = i;
					break;
				}
			}
			(*tile)[dir_n] = dir;
			assert(dir != END);
		}
		(*tile)[dir_n] = END;
	}
	size_t num_tiles = array_len(&array);
	hashmap_t hashmap;
	status_t *tile_status;
	hashmap_init(&hashmap, &tile_status, sizeof (status_t), 10*num_tiles);

	// part 1
	for (size_t idx=0; idx<num_tiles; idx++) {
		tile_t *tile = &tiles[idx];
		coord_t coord = {};
		for (size_t i=0; (*tile)[i] != END; i++) {
			coord = add_dir(coord, (*tile)[i]);
		}
		size_t coord_idx = hashmap_add(&hashmap, &coord, sizeof (coord_t));
		bitmap_init_static(&tile_status[coord_idx].bitmap, 13);
		if (tile_status[coord_idx].color == WHITE) {
			tile_status[coord_idx].color = BLACK;
			res->part1++;
		} else {
			tile_status[coord_idx].color = WHITE;
			res->part1--;
		}
	}

	// part 2
	for (size_t round=0; round<100; round++) {
		hashmap_foreach(&hashmap, run_round, &round);
		hashmap_foreach(&hashmap, finish_round, NULL);
	}
	hashmap_foreach(&hashmap, count_flipped, &res->part2);

	hashmap_free(&hashmap);
	array_free(&array);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {10, 2208}},
		{DAY_INPUT, {254, 3697}},
	};
	return checks;
}
