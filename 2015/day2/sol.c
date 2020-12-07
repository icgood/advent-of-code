#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

typedef struct {
	uint32_t l;
	uint32_t w;
	uint32_t h;
} rect;

static rect *new_array(size_t new_size, rect *old_array, size_t len) {
	rect *new_array = malloc(new_size * sizeof (rect));
	if (len > 0) memcpy(new_array, old_array, len * sizeof (rect));
	free(old_array);
	return new_array;
}

static uint32_t smallest_side(rect *r) {
	uint32_t one = r->l * r->w, two = r->w * r->h, three = r->l * r->h;
	uint32_t one_two = one < two ? one : two;
	return one_two < three ? one_two : three;
}

static uint32_t smallest_perimeter(rect *r) {
	uint32_t one = 2 * r->l + 2 * r->w,
		 two = 2 * r->w + 2 * r->h,
		 three = 2 * r->l + 2 * r->h;
	uint32_t one_two = one < two ? one : two;
	return one_two < three ? one_two : three;
}

void day_result_compute(day_result *res, FILE *in) {
	size_t len = 0, size = 2;
	rect *array = new_array(size, NULL, 0);
	char line[1024];

	while (fgets(line, sizeof (line), in) != NULL) {
		sscanf(line, "%ux%ux%u", &array[len].l, &array[len].w, &array[len].h);
		len++;
		if (len >= size) {
			array = new_array(size *= 2, array, len);
		}
	}

	// part 1
	for (size_t i=0; i<len; i++) {
		rect *r = &array[i];
		res->part1 += smallest_side(r)
			+ 2 * r->l * r->w
			+ 2 * r->w * r->h
			+ 2 * r->l * r->h;
	}

	// part 2
	for (size_t i=0; i<len; i++) {
		rect *r = &array[i];
		res->part2 += smallest_perimeter(r) + r->l * r->w * r->h;
	}

	free(array);
}

void day_answers_provide(day_answers *answers) {
	*answers = (day_answers) {
		{101, 48},
		{1606483, 3842356},
	};
}
