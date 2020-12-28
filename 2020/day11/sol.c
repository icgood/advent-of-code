#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"

typedef char line_buf[1024];

struct seats {
	line_buf *grid;
	size_t width;
	size_t height;
};

typedef int count_adjacent(struct seats *, size_t, size_t);

static int seat_occupied(char seat) {
	return seat == '#' || seat == '>';
}

static int count_adjacent_part1(struct seats *seats, size_t x, size_t y) {
	int count = 0;
	line_buf *grid = seats->grid;
	if (y > 0 && x > 0 && seat_occupied(grid[y-1][x-1])) count++;
	if (y > 0 && seat_occupied(grid[y-1][x])) count++;
	if (y > 0 && seat_occupied(grid[y-1][x+1])) count++;
	if (x > 0 && seat_occupied(grid[y][x-1])) count++;
	if (seat_occupied(grid[y][x+1])) count++;
	if (x > 0 && seat_occupied(grid[y+1][x-1])) count++;
	if (seat_occupied(grid[y+1][x])) count++;
	if (seat_occupied(grid[y+1][x+1])) count++;
	return count;
}

static int count_adjacent_part2(struct seats *seats, size_t x, size_t y) {
	int count = 0;
	size_t width = seats->width, height = seats->height;
	line_buf *grid = seats->grid;
	// W 
	if (x > 0) {
		for (int x2=x-1; x2>=0; x2--) {
			if (seat_occupied(grid[y][x2])) count++;
			if (grid[y][x2] != '.') break;
		}
	}
	// E
	for (int x2=x+1; x2<width; x2++) {
		if (seat_occupied(grid[y][x2])) count++;
		if (grid[y][x2] != '.') break;
	}
	// N
	if (y > 0) {
		for (int y2=y-1; y2>=0; y2--) {
			if (seat_occupied(grid[y2][x])) count++;
			if (grid[y2][x] != '.') break;
		}
	}
	// S
	for (int y2=y+1; y2<height; y2++) {
		if (seat_occupied(grid[y2][x])) count++;
		if (grid[y2][x] != '.') break;
	}
	// NW
	if (x > 0 && y > 0) {
		for (int x2=x-1, y2=y-1; x2>=0 && y2>=0; x2--, y2--) {
			if (seat_occupied(grid[y2][x2])) count++;
			if (grid[y2][x2] != '.') break;
		}
	}
	// NE
	if (y > 0) {
		for (int x2=x+1, y2=y-1; x2<width && y2>=0; x2++, y2--) {
			if (seat_occupied(grid[y2][x2])) count++;
			if (grid[y2][x2] != '.') break;
		}
	}
	// SW
	if (x > 0) {
		for (int  x2=x-1, y2=y+1; x2>=0 && y2<height; x2--, y2++) {
			if (seat_occupied(grid[y2][x2])) count++;
			if (grid[y2][x2] != '.') break;
		}
	}
	// SE
	for (int x2=x+1, y2=y+1; x2<width && y2<height; x2++, y2++) {
		if (seat_occupied(grid[y2][x2])) count++;
		if (grid[y2][x2] != '.') break;
	}
	return count;
}

static void swap_all(struct seats *seats, char from, char to) {
	size_t width = seats->width, height = seats->height;
	line_buf *grid = seats->grid;
	for (size_t y=0; y<height; y++) {
		for (size_t x=0; x<width; x++) {
			if (grid[y][x] == from) grid[y][x] = to;
		}
	}
}

static int count_all(struct seats *seats, char val) {
	int count = 0;
	size_t width = seats->width, height = seats->height;
	line_buf *grid = seats->grid;
	for (size_t y=0; y<height; y++) {
		for (size_t x=0; x<width; x++) {
			if (grid[y][x] == val) count++;
		}
	}
	return count;
}

static int apply_rules(struct seats *seats, count_adjacent count_func, int threshold) {
	int changes = 0;
	size_t width = seats->width, height = seats->height;
	line_buf *grid = seats->grid;
	for (size_t y=0; y<height; y++) {
		for (size_t x=0; x<width; x++) {
			if (grid[y][x] == '.') continue;
			int adjacent = count_func(seats, x, y);
			if (adjacent == 0 && grid[y][x] == 'L') {
				grid[y][x] = '<';
				changes++;
			} else if (adjacent >= threshold && grid[y][x] == '#') {
				grid[y][x] = '>';
				changes++;
			}
		}
	}
	return changes;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	struct array_data array;
	struct seats seats;
	line_buf line;
	array_init(&array, &seats.grid, sizeof (line_buf));

	while (fgets(line, sizeof (line_buf), in) != NULL) {
		size_t idx = array_add(&array);
		memcpy(seats.grid[idx], line, sizeof (line_buf));
	}
	seats.width = strspn(seats.grid[0], "L.");
	seats.height = array_len(&array);
	array_add(&array);

	// part 1
	while (1) {
		int changes = apply_rules(&seats, count_adjacent_part1, 4);
		swap_all(&seats, '<', '#');
		swap_all(&seats, '>', 'L');
		if (changes == 0) {
			res->part1 = count_all(&seats, '#');
			break;
		}
	}
	swap_all(&seats, '#', 'L');

	// part 2
	while (1) {
		int changes = apply_rules(&seats, count_adjacent_part2, 5);
		swap_all(&seats, '<', '#');
		swap_all(&seats, '>', 'L');
		if (changes == 0) {
			res->part2 = count_all(&seats, '#');
			break;
		}
	}
	swap_all(&seats, '#', 'L');

	array_free(&array);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {37, 26}},
		{DAY_INPUT, {2453, 2159}},
	};
	return checks;
}
