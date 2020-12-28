#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day_base/struct/hashmap.h"
#include "day_base/struct/bitmap.h"

typedef unsigned int id_t;
typedef enum { BLACK = '.', WHITE = '#' } color_t;
typedef enum { TOP = 0, RIGHT = 1, BOTTOM = 2, LEFT = 3 } border_side_t;
typedef enum { EDGE, TILE } neighbor_type_t;

typedef struct {
	struct bitmap_data bitmap;
	unsigned char data[2];
} border_t;

typedef struct {
	struct bitmap_data bitmap;
	unsigned char data[1];
} image_row_t;

typedef image_row_t image_t[8];

typedef struct {
	neighbor_type_t type;
	size_t idx;
	border_side_t side;
} neighbor_t;

typedef struct {
	id_t id;
	image_t image;
	border_t borders[4];
	neighbor_t neighbors[4];
	size_t neighbor_tiles;
} tile_t;

typedef struct {
	size_t owners[4];
	size_t count;
} border_match_t;

const size_t sea_monster_size = 15;
const char sea_monster_grid[3][21] = {
	"..................#.",
	"#....##....##....###",
	".#..#..#..#..#..#...",
};

typedef struct bitmap_data sea_monster_t[3];

static void parse_tile(tile_t *tile, char (*grid)[10]) {
	for (border_side_t side=0; side<4; side++) {
		bitmap_init_static(&tile->borders[side].bitmap, 10);
	}

	for (size_t i=0; i<10; i++) {
		if (grid[0][i] == WHITE) bitmap_set(&tile->borders[TOP].bitmap, i);
	}
	for (size_t i=0; i<10; i++) {
		if (grid[i][9] == WHITE) bitmap_set(&tile->borders[RIGHT].bitmap, i);
	}
	for (size_t i=0; i<10; i++) {
		if (grid[9][i] == WHITE) bitmap_set(&tile->borders[BOTTOM].bitmap, 9 - i);
	}
	for (size_t i=0; i<10; i++) {
		if (grid[i][0] == WHITE) bitmap_set(&tile->borders[LEFT].bitmap, 9 - i);
	}

	for (size_t y=0; y<8; y++) {
		bitmap_init_static(&tile->image[y].bitmap, 8);
		for (size_t x=0; x<8; x++) {
			if (grid[y+1][x+1] == WHITE) bitmap_set(&tile->image[y].bitmap, x);
		}
	}
}

static void parse_sea_monster(sea_monster_t *monster) {
	for (size_t row=0; row<3; row++) {
		struct bitmap_data *dest_row = &(*monster)[row];
		bitmap_init(dest_row, 20);
		for (size_t col=0; col<20; col++) {
			if (sea_monster_grid[row][col] == WHITE) {
				bitmap_set(dest_row, col);
			}
		}
	}
}

static void flip_bits(struct bitmap_data *bitmap, size_t num_bits) {
	for (size_t left_pos=0; left_pos<num_bits/2; left_pos++) {
		size_t right_pos = num_bits - left_pos - 1;
		int left = bitmap_get(bitmap, left_pos);
		int right = bitmap_get(bitmap, right_pos);
		bitmap_assign(bitmap, left_pos, right);
		bitmap_assign(bitmap, right_pos, left);
	}
}

static border_t flip_border(border_t border) {
	flip_bits(&border.bitmap, 10);
	return border;
}

static void rotate_tile(tile_t *tile) {
	image_t copy;
	memcpy(&copy, &tile->image, sizeof (image_t));
	for (size_t y=0; y<8; y++) bitmap_clear(&tile->image[y].bitmap);
	for (size_t y=0; y<8; y++) {
		for (size_t x=0; x<8; x++) {
			if (bitmap_get(&copy[7-y].bitmap, x)) {
				bitmap_set(&tile->image[x].bitmap, y);
			}
		}
	}
	border_t b_copy = tile->borders[TOP];
	tile->borders[TOP] = tile->borders[LEFT];
	tile->borders[LEFT] = tile->borders[BOTTOM];
	tile->borders[BOTTOM] = tile->borders[RIGHT];
	tile->borders[RIGHT] = b_copy;
	neighbor_t n_copy = tile->neighbors[TOP];
	tile->neighbors[TOP] = tile->neighbors[LEFT];
	tile->neighbors[LEFT] = tile->neighbors[BOTTOM];
	tile->neighbors[BOTTOM] = tile->neighbors[RIGHT];
	tile->neighbors[RIGHT] = n_copy;
}

static void flip_tile_horizontal(tile_t *tile) {
	for (size_t y=0; y<8; y++) {
		flip_bits(&tile->image[y].bitmap, 8);
	}
	tile->borders[TOP] = flip_border(tile->borders[TOP]);
	tile->borders[BOTTOM] = flip_border(tile->borders[BOTTOM]);
	border_t b_copy = tile->borders[LEFT];
	tile->borders[LEFT] = flip_border(tile->borders[RIGHT]);
	tile->borders[RIGHT] = flip_border(b_copy);
	neighbor_t n_copy = tile->neighbors[LEFT];
	tile->neighbors[LEFT] = tile->neighbors[RIGHT];
	tile->neighbors[RIGHT] = n_copy;
}

static void flip_tile_vertical(tile_t *tile) {
	for (size_t y=0; y<4; y++) {
		image_row_t row = tile->image[y];
		tile->image[y] = tile->image[7-y];
		tile->image[7-y] = row;
	}
	tile->borders[LEFT] = flip_border(tile->borders[LEFT]);
	tile->borders[RIGHT] = flip_border(tile->borders[RIGHT]);
	border_t b_copy = tile->borders[TOP];
	tile->borders[TOP] = flip_border(tile->borders[BOTTOM]);
	tile->borders[BOTTOM] = flip_border(b_copy);
	neighbor_t copy = tile->neighbors[TOP];
	tile->neighbors[TOP] = tile->neighbors[BOTTOM];
	tile->neighbors[BOTTOM] = copy;
}

static int get_neighbor(tile_t *tiles, size_t idx, border_side_t side, neighbor_t *neighbor, struct hashmap_data *hashmap, border_match_t *border_matches) {
	tile_t *tile = &tiles[idx];
	border_t border = tile->borders[side];
	size_t *match_idx = hashmap_lookup(hashmap, bitmap_pointer(&border.bitmap), bitmap_sizeof(&border.bitmap));
	border_t flipped_border = flip_border(border);
	size_t *flipped_match_idx = hashmap_lookup(hashmap, bitmap_pointer(&flipped_border.bitmap), bitmap_sizeof(&flipped_border.bitmap));
	if (match_idx != NULL && flipped_match_idx == NULL) {
		border_match_t *match = &border_matches[*match_idx];
		if (match->count == 1) {
			assert(match->owners[0] == idx);
			neighbor->type = EDGE;
		} else {
			neighbor->type = TILE;
			neighbor->idx = match->owners[0] != idx ? match->owners[0] : match->owners[1];
			neighbor->side = side;
		}
	} else if (flipped_match_idx != NULL) {
		border_match_t *match = &border_matches[*flipped_match_idx];
		assert(match->count == 1);
		neighbor->type = TILE;
		neighbor->idx = match->owners[0];
		neighbor->side = side;
	} else {
		assert(!"matched both flipped and un-flipped");
	}
	return neighbor->type == TILE ? 1 : 0;
}

static void process_tile(tile_t *tiles, size_t idx, struct hashmap_data *hashmap, border_match_t *border_matches) {
	tile_t *tile = &tiles[idx];
	for (border_side_t side=0; side<4; side++) {
		if (get_neighbor(tiles, idx, side, &tile->neighbors[side], hashmap, border_matches)) {
			tile->neighbor_tiles++;
		}
	}
}

static size_t find_top_left(tile_t *tiles, size_t tiles_len) {
	size_t idx;
	for (idx=0; idx<tiles_len; idx++) {
		if (tiles[idx].neighbor_tiles == 2) break;
	}
	assert(idx < tiles_len);
	tile_t *tile = &tiles[idx];
	if (tile->neighbors[TOP].type == TILE) flip_tile_vertical(tile);
	if (tile->neighbors[LEFT].type == TILE) flip_tile_horizontal(tile);
	return idx;
}

static void reorient_neighbors(tile_t *tiles, size_t idx, struct bitmap_data *seen) {
	if (bitmap_get(seen, idx)) return;
	else bitmap_set(seen, idx);
	tile_t *tile = &tiles[idx];
	for (border_side_t side=0; side<4; side++) {
		neighbor_t *neighbor = &tile->neighbors[side];
		if (neighbor->type != TILE) continue;
		border_t border = tile->borders[side];
		border_t flipped_border = flip_border(border);
		tile_t *neighbor_tile = &tiles[neighbor->idx];
		border_side_t rotate, opposite = (side + 2) % 4;
		for (rotate=0; rotate<4; rotate++) {
			border_t neighbor_border = neighbor_tile->borders[opposite];
			if (bitmap_cmp(&neighbor_border.bitmap, &border.bitmap) == 0) {
				if (opposite % 2 == 0) flip_tile_horizontal(neighbor_tile);
				else flip_tile_vertical(neighbor_tile);
				break;
			} else if (bitmap_cmp(&neighbor_border.bitmap, &flipped_border.bitmap) == 0) {
				break;
			}
			
			rotate_tile(neighbor_tile);
		}
		assert(rotate < 4);
		reorient_neighbors(tiles, neighbor->idx, seen);
	}
}

static size_t count_monsters(sea_monster_t *monster, struct bitmap_data *board, size_t dim) {
	size_t num_monsters = 0;
	for (size_t board_y=0; board_y<dim-2; board_y++) {
		for (size_t board_x=0; board_x<dim; board_x++) {
			if (bitmap_bitand(&board[board_y], board_x, &(*monster)[0])
					&& bitmap_bitand(&board[board_y+1], board_x, &(*monster)[1])
					&& bitmap_bitand(&board[board_y+2], board_x, &(*monster)[2])) {
				num_monsters++;
			}
		}
	}
	return num_monsters;
}

static void flip_board_horizontal(struct bitmap_data *board, size_t dim) {
	for (size_t row=0; row<dim; row++) {
		flip_bits(&board[row], dim);
	}
}

static void flip_board_vertical(struct bitmap_data *board, size_t dim) {
	for (size_t col=0; col<dim; col++) {
		for (size_t row=0; row<dim/2; row++) {
			int left = bitmap_get(&board[row], col);
			int right = bitmap_get(&board[dim - row - 1], col);
			bitmap_assign(&board[row], col, right);
			bitmap_assign(&board[dim - row - 1], col, left);
		}
	}
}

static void rotate_board(struct bitmap_data *board, size_t dim) {
	struct bitmap_data board_copy[dim];
	for (size_t row=0; row<dim; row++) {
		bitmap_init(&board_copy[row], dim);
		for (size_t col=0; col<dim; col++) {
			if (bitmap_get(&board[row], col)) bitmap_set(&board_copy[row], col);
		}
	}
	for (size_t row=0; row<dim; row++) {
		for (size_t col=0; col<dim; col++) {
			int val = bitmap_get(&board_copy[dim-row-1], col);
			bitmap_assign(&board[col], row, val);
		}
	}
	for (size_t row=0; row<dim; row++) bitmap_free(&board_copy[row]);
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	char line[1024];
	struct array_data array;
	tile_t *tiles;

	array_init(&array, &tiles, sizeof (tile_t));
	while (fgets(line, sizeof (line), in) != NULL) {
		assert(strncmp(line, "Tile ", 5) == 0);
		char grid[10][10];
		size_t idx = array_add(&array);
		tiles[idx].id = atoi(line + 5);
		for (size_t y=0; y<10; y++) {
			assert(fgets(line, sizeof (line), in) != NULL);
			for (size_t x=0; x<10; x++) {
				grid[y][x] = line[x];
			}
			assert(line[10] == '\n');
		}
		fgets(line, sizeof (line), in);  // empty line
		parse_tile(&tiles[idx], grid);
	}
	size_t tiles_len = array_len(&array);
	size_t board_dim = sqrt(tiles_len) * 8;

	// part 1
	struct hashmap_data hashmap;
	border_match_t *border_matches;
	hashmap_init(&hashmap, &border_matches, sizeof (border_match_t), tiles_len * 4);
	for (size_t idx=0; idx<tiles_len; idx++) {
		for (border_side_t side=0; side<4; side++) {
			border_t border = tiles[idx].borders[side];
			size_t match_idx = hashmap_add(&hashmap, bitmap_pointer(&border.bitmap), bitmap_sizeof(&border.bitmap));
			border_match_t *match = &border_matches[match_idx];
			match->owners[match->count++] = idx;
		}
	}
	res->part1 = 1;
	for (size_t idx=0; idx<tiles_len; idx++) {
		process_tile(tiles, idx, &hashmap, border_matches);
		if (tiles[idx].neighbor_tiles == 2) res->part1 *= tiles[idx].id;
	}
	hashmap_free(&hashmap);

	// part 2
	size_t top_left_idx = find_top_left(tiles, tiles_len);
	struct bitmap_data seen;
	bitmap_init(&seen, tiles_len);
	reorient_neighbors(tiles, top_left_idx, &seen);
	bitmap_free(&seen);

	struct bitmap_data board[board_dim];
	tile_t *cur_tile = &tiles[top_left_idx];
	for (size_t board_y=0; board_y<board_dim/8; board_y++) {
		tile_t *row_head = cur_tile;
		for (size_t y=0; y<8; y++) bitmap_init(&board[(board_y * 8) + y], board_dim);
		for (size_t board_x=0; board_x<board_dim/8; board_x++) {
			for (size_t y=0; y<8; y++) {
				for (size_t x=0; x<8; x++) {
					if (bitmap_get(&cur_tile->image[y].bitmap, x)) {
						bitmap_set(&board[(board_y * 8) + y], (board_x * 8) + x);
					}
				}
			}
			cur_tile = &tiles[cur_tile->neighbors[RIGHT].idx];
		}
		cur_tile = &tiles[row_head->neighbors[BOTTOM].idx];
	}

	sea_monster_t monster;
	parse_sea_monster(&monster);
	size_t num_monsters = 0;

	for (int rotate=0; rotate<4; rotate++) {
		if ((num_monsters = count_monsters(&monster, board, board_dim)) > 0) break;
		flip_board_horizontal(board, board_dim);
		if ((num_monsters = count_monsters(&monster, board, board_dim)) > 0) break;
		flip_board_horizontal(board, board_dim);
		flip_board_vertical(board, board_dim);
		if ((num_monsters = count_monsters(&monster, board, board_dim)) > 0) break;
		flip_board_horizontal(board, board_dim);
		if ((num_monsters = count_monsters(&monster, board, board_dim)) > 0) break;
		flip_board_horizontal(board, board_dim);
		flip_board_vertical(board, board_dim);
		rotate_board(board, board_dim);
	}

	size_t num_white = 0;
	for (size_t board_y=0; board_y<board_dim; board_y++) {
		num_white += bitmap_count(&board[board_y]);
	}
	res->part2 = num_white - (num_monsters * sea_monster_size);

	for (size_t i=0; i<board_dim; i++) bitmap_free(&board[i]);
	for (size_t i=0; i<3; i++) bitmap_free(&monster[i]);
	array_free(&array);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {20899048083289, 273}},
		{DAY_INPUT, {83775126454273, 1993}},
	};
	return checks;
}
