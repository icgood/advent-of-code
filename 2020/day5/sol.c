#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

static uint16_t *new_array(size_t new_size, uint16_t *old_array, size_t len) {
	uint16_t *new_array = malloc(new_size * sizeof (uint16_t));
	if (len > 0) memcpy(new_array, old_array, len * sizeof (uint16_t));
	free(old_array);
	return new_array;
}

static uint16_t get_seat_id(char *line) {
	if (strspn(line, "FB") < 7 || strspn(line+7, "LR") < 3) {
		return 0;
	}

	uint16_t row_id = 0, col_id = 0;
	if (line[0] == 'B') row_id |= (1 << 6);
	if (line[1] == 'B') row_id |= (1 << 5);
	if (line[2] == 'B') row_id |= (1 << 4);
	if (line[3] == 'B') row_id |= (1 << 3);
	if (line[4] == 'B') row_id |= (1 << 2);
	if (line[5] == 'B') row_id |= (1 << 1);
	if (line[6] == 'B') row_id |= (1 << 0);
	if (line[7] == 'R') col_id |= (1 << 2);
	if (line[8] == 'R') col_id |= (1 << 1);
	if (line[9] == 'R') col_id |= (1 << 0);
	return row_id * 8 + col_id;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	char line[1024];
	size_t array_len = 0, array_size = 2;
	uint16_t *array = new_array(array_size, NULL, 0);

	while (fgets(line, sizeof (line), in) != NULL) {
		array[array_len++] = get_seat_id(line);
		if (array_len >= array_size) {
			array = new_array(array_size *= 2, array, array_len);
		}
	}

	// part 1
	for (size_t i=0; i<array_len; i++) {
		if (array[i] > res->part1) {
			res->part1 = array[i];
		}
	}

	// part 2
	uint8_t bitmap[128];
	memset(bitmap, 0, sizeof (bitmap));
	for (size_t i=0; i<array_len; i++) {
		uint16_t val = array[i];
		size_t pos_byte = val / 8;
		uint8_t pos_bit = val % 8;
		bitmap[pos_byte] |= (1 << pos_bit);
	}
	for (uint16_t i=8; i<1016; i++) {
		size_t pos_byte = i / 8;
		uint8_t pos_bit = i % 8;
		size_t left_pos_byte = (i-1) / 8;
		uint8_t left_pos_bit = (i-1) % 8;
		size_t right_pos_byte = (i+1) / 8;
		uint8_t right_pos_bit = (i+1) % 8;

		int seat_free = (bitmap[pos_byte] & (1 << pos_bit)) == 0;
		int left_seat_taken = (bitmap[left_pos_byte] & (1 << left_pos_bit)) != 0;
		int right_seat_taken = (bitmap[right_pos_byte] & (1 << right_pos_bit)) != 0;
		if (seat_free && left_seat_taken && right_seat_taken) {
			res->part2 = i;
		}
	}

	free(array);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {820, 0}},
		{DAY_INPUT, {965, 524}},
	};
	return checks;
}
