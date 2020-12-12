#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"

typedef char line_buf[1024];

static int seat_occupied(char seat) {
	return seat == '#' || seat == '>';
}

static int count_adjacent_part1(line_buf *seats, size_t width, size_t height, size_t x, size_t y) {
	int count = 0;
	if (y > 0 && x > 0 && seat_occupied(seats[y-1][x-1])) count++;
	if (y > 0 && seat_occupied(seats[y-1][x])) count++;
	if (y > 0 && seat_occupied(seats[y-1][x+1])) count++;
	if (x > 0 && seat_occupied(seats[y][x-1])) count++;
	if (seat_occupied(seats[y][x+1])) count++;
	if (x > 0 && seat_occupied(seats[y+1][x-1])) count++;
	if (seat_occupied(seats[y+1][x])) count++;
	if (seat_occupied(seats[y+1][x+1])) count++;
	return count;
}

static int count_adjacent_part2(line_buf *seats, size_t width, size_t height, size_t x, size_t y) {
	int count = 0;
	// W 
	if (x > 0) {
		for (size_t x2=x-1; x2>=0; x2--) {
			if (seat_occupied(seats[y][x2])) count++;
			if (seats[y][x2] != '.') break;
		}
	}
	// E
	for (size_t x2=x+1; x2<width; x2++) {
		if (seat_occupied(seats[y][x2])) count++;
		if (seats[y][x2] != '.') break;
	}
	// N
	if (y > 0) {
		for (size_t y2=y-1; y2>=0; y2--) {
			if (seat_occupied(seats[y2][x])) count++;
			if (seats[y2][x] != '.') break;
		}
	}
	// S
	for (size_t y2=y+1; y2<height; y2++) {
		if (seat_occupied(seats[y2][x])) count++;
		if (seats[y2][x] != '.') break;
	}
	// NW
	if (x > 0 && y > 0) {
		for (size_t x2=x-1, y2=y-1; x2>=0 && y2>=0; x2--, y2--) {
			if (seat_occupied(seats[y2][x2])) count++;
			if (seats[y2][x2] != '.') break;
		}
	}
	// NE
	if (y > 0) {
		for (size_t x2=x+1, y2=y-1; x2<width && y2>=0; x2++, y2--) {
			if (seat_occupied(seats[y2][x2])) count++;
			if (seats[y2][x2] != '.') break;
		}
	}
	// SW
	if (x > 0) {
		for (size_t x2=x-1, y2=y+1; x2>=0 && y2<height; x2--, y2++) {
			if (seat_occupied(seats[y2][x2])) count++;
			if (seats[y2][x2] != '.') break;
		}
	}
	// SE
	for (size_t x2=x+1, y2=y+1; x2<width && y2<height; x2++, y2++) {
		if (seat_occupied(seats[y2][x2])) count++;
		if (seats[y2][x2] != '.') break;
	}
	return count;
}

static void swap_all(line_buf *seats, size_t width, size_t height, char from, char to) {
	for (size_t y=0; y<height; y++) {
		for (size_t x=0; x<width; x++) {
			if (seats[y][x] == from) seats[y][x] = to;
		}
	}
}

static int count_all(line_buf *seats, size_t width, size_t height, char val) {
	int count = 0;
	for (size_t y=0; y<height; y++) {
		for (size_t x=0; x<width; x++) {
			if (seats[y][x] == val) count++;
		}
	}
	return count;
}

static int apply_rules_part1(line_buf *seats, size_t width, size_t height) {
	int changes = 0;
	for (size_t y=0; y<height; y++) {
		for (size_t x=0; x<width; x++) {
			if (seats[y][x] == '.') continue;
			int adjacent = count_adjacent_part1(seats, width, height, x, y);
			if (adjacent == 0 && seats[y][x] == 'L') {
				seats[y][x] = '<';
				changes++;
			} else if (adjacent >= 4 && seats[y][x] == '#') {
				seats[y][x] = '>';
				changes++;
			}
		}
	}
	return changes;
}

static int apply_rules_part2(line_buf *seats, size_t width, size_t height) {
	int changes = 0;
	for (size_t y=0; y<height; y++) {
		for (size_t x=0; x<width; x++) {
			if (seats[y][x] == '.') continue;
			int adjacent = count_adjacent_part2(seats, width, height, x, y);
			if (adjacent == 0 && seats[y][x] == 'L') {
				seats[y][x] = '<';
				changes++;
			} else if (adjacent >= 5 && seats[y][x] == '#') {
				seats[y][x] = '>';
				changes++;
			}
		}
	}
	return changes;
}

void day_result_compute(char *arg, day_result *res, FILE *in) {
	struct array_data array;
	line_buf *seats;
	line_buf line;
	array_init(&array, &seats, 1024);

	while (fgets(line, 1024, in) != NULL) {
		size_t idx = array_add(&array);
		memcpy(seats[idx], line, 1024);
	}
	size_t width = strspn(seats[0], "L.");
	size_t height = array_len(&array);
	array_add(&array);

	// part 1
	while (1) {
		int changes = apply_rules_part1(seats, width, height);
		swap_all(seats, width, height, '<', '#');
		swap_all(seats, width, height, '>', 'L');
		if (changes == 0) {
			res->part1 = count_all(seats, width, height, '#');
			break;
		}
	}
	swap_all(seats, width, height, '#', 'L');

	// part 2
	while (1) {
		int changes = apply_rules_part2(seats, width, height);
		swap_all(seats, width, height, '<', '#');
		swap_all(seats, width, height, '>', 'L');
		if (changes == 0) {
			res->part2 = count_all(seats, width, height, '#');
			break;
		}
	}
	swap_all(seats, width, height, '#', 'L');

	array_free(&array);
}

void day_answers_provide(day_arguments *args, day_answers *answers) {
	*answers = (day_answers) {
		{37, 26},
		{2453, 2159},
	};
}
