#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"

typedef unsigned long long timestamp_t;

typedef struct {
	timestamp_t id;
	timestamp_t offset;
} bus_t;

void day_result_compute(char *arg, day_result *res, FILE *in) {
	char line[1024], *ptr;
	timestamp_t earliest, offset = 0;
	struct array_data array;
	bus_t *buses;

	assert(fscanf(in, "%llu\n", &earliest) > 0);
	assert(fgets(line, sizeof (line), in) != NULL);

	ptr = line;
	array_init(&array, &buses, sizeof (bus_t));
	do {
		if (*ptr == 'x') {
			ptr += 1;
		} else {
			size_t idx = array_add(&array);
			buses[idx].id = atol(ptr);
			buses[idx].offset = offset;
			ptr += strspn(ptr, "0123456789");
		}
		offset++;
		if (*ptr == ',') ptr++;
	} while (*ptr != '\0' && *ptr != '\n');

	// part 1
	timestamp_t current = earliest;
	while (res->part1 == 0) {
		for (size_t i=0; i<array_len(&array); i++) {
			if (current % buses[i].id == 0) {
				res->part1 = buses[i].id * (current - earliest);
			};
		}
		current++;
	}

	// part 2
	timestamp_t ts = 0, interval = buses[0].id;
	for (size_t i=1; i<array_len(&array); i++) {
		bus_t *bus = &buses[i];
		while ((ts + bus->offset) % bus->id != 0) {
			ts += interval;
		}
		interval *= bus->id;
	}
	res->part2 = ts;

	array_free(&array);
}

void day_answers_provide(day_arguments *args, day_answers *answers) {
	*answers = (day_answers) {
		{295, 1068781},
		{2305, 552612234243498},
	};
}
