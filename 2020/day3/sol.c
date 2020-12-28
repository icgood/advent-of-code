#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

#define LINE_BUF 1024

static char **new_grid(size_t new_size, char **old_grid, size_t old_size) {
	char **grid = malloc(new_size * sizeof(char*));
	for (size_t i=0; i<old_size; i++) {
		grid[i] = old_grid[i];
	}
	free(old_grid);
	for (size_t i=old_size; i<new_size; i++) {
		grid[i] = malloc(LINE_BUF);
	}
	return grid;
}

static void free_grid(char **grid, size_t size) {
	for (size_t i=0; i<size; i++) {
		free(grid[i]);
	}
	free(grid);
}

static int is_tree(char **grid, size_t grid_width, size_t x, size_t y) {
	return grid[y][x % grid_width] == '#' ? 1 : 0;
}

static size_t num_trees(char **grid, size_t grid_len, size_t grid_width, size_t slope_x, size_t slope_y) {
	size_t x = 0, y = 0, ret = 0;
	while (y < grid_len) {
		if (is_tree(grid, grid_width, x, y) == 1) {
			ret++;
		}
		x += slope_x;
		y += slope_y;
	}
	return ret;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	size_t grid_len = 0;
	size_t grid_size = 2;
	char **grid = new_grid(grid_size, NULL, 0);

	while (fgets(grid[grid_len], 1024, in) != NULL) {
		grid_len++;
		if (grid_len >= grid_size) {
			grid = new_grid(grid_size *= 2, grid, grid_len);
		}
	}
	size_t grid_width = strspn(grid[0], ".#");

	// part 1
	res->part1 = num_trees(grid, grid_len, grid_width, 3, 1);

	// part 2
	res->part2 = num_trees(grid, grid_len, grid_width, 1, 1) *
		num_trees(grid, grid_len, grid_width, 3, 1) *
		num_trees(grid, grid_len, grid_width, 5, 1) *
		num_trees(grid, grid_len, grid_width, 7, 1) *
		num_trees(grid, grid_len, grid_width, 1, 2);

	free_grid(grid, grid_size);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {7, 336}},
		{DAY_INPUT, {216, 6708199680}},
	};
	return checks;
}
