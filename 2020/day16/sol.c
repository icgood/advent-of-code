#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day_base/struct/bitmap.h"

typedef unsigned int number_t;
typedef number_t ticket_t[32];

typedef struct {
	char name[64];
	struct bitmap_data ranges;
	struct bitmap_data possibilities;
} field_t;

static void parse_ranges(char *line, struct bitmap_data *ranges, struct bitmap_data *all_ranges) {
	number_t begin = atoi(line);
	line += strspn(line, "0123456789");
	assert(line[0] == '-');
	line++;
	number_t end = atoi(line);
	line += strspn(line, "0123456789");
	if (strncmp(line, " or ", 4) == 0) {
		parse_ranges(line + 4, ranges, all_ranges);
	}
	for (number_t i=begin; i<=end; i++) {
		bitmap_resize(ranges, i);
		bitmap_resize(all_ranges, i);
		bitmap_set(ranges, i);
		bitmap_set(all_ranges, i);
	}
}

static void parse_ticket(char *line, ticket_t *ticket) {
	size_t i = 0;
	char *ptr = line;
	while (*ptr != '\n' && *ptr != '\0') {
		(*ticket)[i++] = atoi(ptr);
		ptr += strspn(ptr, "0123456789");
		ptr += strspn(ptr, ",");
	}
}

void day_result_compute(char *arg, day_result *res, FILE *in) {
	char line[1024];
	struct array_data array_fields;
	struct array_data array_tickets;
	field_t *fields;
	struct bitmap_data all_ranges;
	struct bitmap_data invalid_tickets;
	ticket_t my_ticket;
	ticket_t *nearby_tickets;

	array_init(&array_fields, &fields, sizeof (field_t));
	array_init(&array_tickets, &nearby_tickets, sizeof (ticket_t));
	bitmap_init(&all_ranges, 0);
	while (fgets(line, sizeof (line), in) != NULL) {
		size_t key_len = strspn(line, "abcdefghijklmnopqrstuvwxyz ");
		if (key_len == 0) break;
		size_t idx = array_add(&array_fields);
		memcpy(fields[idx].name, line, key_len);
		fields[idx].name[key_len] = '\0';
		bitmap_init(&fields[idx].ranges, 0);
		assert(strncmp(line + key_len, ": ", 2) == 0);
		parse_ranges(line + key_len + 2, &fields[idx].ranges, &all_ranges);
	}
	assert(fgets(line, sizeof (line), in) != NULL);
	assert(strcmp(line, "your ticket:\n") == 0);
	assert(fgets(line, sizeof (line), in) != NULL);
	parse_ticket(line, &my_ticket);
	assert(fgets(line, sizeof (line), in) != NULL);
	assert(strcmp(line, "\n") == 0);
	assert(fgets(line, sizeof (line), in) != NULL);
	assert(strcmp(line, "nearby tickets:\n") == 0);
	while (fgets(line, sizeof (line), in) != NULL) {
		size_t idx = array_add(&array_tickets);
		parse_ticket(line, &nearby_tickets[idx]);
	}
	size_t fields_len = array_len(&array_fields);
	size_t tickets_len = array_len(&array_tickets);
	bitmap_init(&invalid_tickets, tickets_len);

	// part 1
	for (size_t i=0; i<tickets_len; i++) {
		for (size_t j=0; j<fields_len; j++) {
			if (!bitmap_get(&all_ranges, nearby_tickets[i][j])) {
				bitmap_set(&invalid_tickets, i);
				res->part1 += nearby_tickets[i][j];
				break;
			}
		}
	}

	// part 2
	res->part2 = 1;
	for (size_t idx=0; idx<fields_len; idx++) {
		field_t *field = &fields[idx];
		bitmap_init(&field->possibilities, fields_len);
		for (size_t j=0; j<fields_len; j++) {
			int all_valid = 1;
			for (size_t i=0; i<tickets_len; i++) {
				if (!bitmap_get(&invalid_tickets, i)) {
					if (!bitmap_get(&field->ranges, nearby_tickets[i][j])) {
						all_valid = 0;
						break;
					}
				}
			}
			if (all_valid) {
				bitmap_set(&field->possibilities, j);
			}
		}
	}
	struct bitmap_data determined;
	bitmap_init(&determined, fields_len);
	while (bitmap_count(&determined) < fields_len) {
		for (size_t idx=0; idx<fields_len; idx++) {
			if (!bitmap_get(&determined, idx)) {
				field_t *field = &fields[idx];
				if (bitmap_count(&field->possibilities) == 1) {
					size_t col = bitmap_max(&field->possibilities);
					bitmap_set(&determined, idx);
					for (size_t j=0; j<fields_len; j++) {
						bitmap_unset(&fields[j].possibilities, col);
					}
					if (strncmp(field->name, "departure", 9) == 0) {
						res->part2 *= my_ticket[col];
					}
				}
			}
		}
	}
	bitmap_free(&determined);

	for (size_t i=0; i<fields_len; i++) {
		bitmap_free(&fields[i].ranges);
		bitmap_free(&fields[i].possibilities);
	}
	bitmap_free(&all_ranges);
	bitmap_free(&invalid_tickets);
	array_free(&array_fields);
	array_free(&array_tickets);
}

void day_answers_provide(day_arguments *args, day_answers *answers) {
	*answers = (day_answers) {
		{71, 1},
		{23954, 453459307723},
	};
}
