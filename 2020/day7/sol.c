#include <assert.h>
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/hashmap.h"

#define COLOR_MAX 64
#define CONTAINS_MAX 16
#define NUM_BUCKETS 1024 

static void copy_color(char *dest, char *line, regmatch_t *m) {
	size_t len = m->rm_eo - m->rm_so;
	size_t n = len <= COLOR_MAX ? len : COLOR_MAX;
	memset(dest, 0, COLOR_MAX);
	strncpy(dest, line + m->rm_so, n);
}

typedef struct {
	char color[COLOR_MAX];
	int n;
} contained_color;

typedef struct {
	char color[COLOR_MAX];
	contained_color contains[CONTAINS_MAX];
	size_t contains_len;
} color;

static int can_carry(color *c, char *wanted, struct hashmap_data *colors, color *array) {
	int total = 0;
	for (size_t i=0; i<c->contains_len; i++) {
		contained_color *contained = &c->contains[i];
		size_t idx = *hashmap_lookup(colors, contained->color, strlen(contained->color));
		color *next_c = &array[idx];
		if (strcmp(contained->color, wanted) == 0) total += contained->n;
		else total += can_carry(next_c, wanted, colors, array);
	}
	return total;
}

static int count_contained(color *c, struct hashmap_data *colors, color *array) {
	int total = 0;
	for (size_t i=0; i<c->contains_len; i++) {
		contained_color *contained = &c->contains[i];
		size_t idx = *hashmap_lookup(colors, contained->color, strlen(contained->color));
		color *next_c = &array[idx];
		total += contained->n + contained->n * count_contained(next_c, colors, array);
	}
	return total;
}

void day_result_compute(day_result *res, FILE *in) {
	char line[1024];
	struct hashmap_data colors;
	color *array;

	hashmap_init(&colors, &array, sizeof (color), NUM_BUCKETS);

	regex_t start_re, contained_re;
	regmatch_t start_matches[2], contained_matches[3];
	assert(regcomp(&start_re, "^([a-z]+ [a-z]+) bags contain", REG_EXTENDED) == 0);
	assert(regcomp(&contained_re, " ([0-9]+) ([a-z]+ [a-z]+) bags?[,.]", REG_EXTENDED) == 0);

	while (fgets(line, sizeof (line), in) != NULL) {
		if (regexec(&start_re, line, 2, start_matches, 0) != 0) break;
		const char *key = line + start_matches[1].rm_so;
		size_t key_len = start_matches[1].rm_eo - start_matches[1].rm_so;
		size_t idx = hashmap_add(&colors, key, key_len);

		copy_color(array[idx].color, line, &start_matches[1]);
		char *after = line + start_matches[0].rm_eo;
		while (regexec(&contained_re, after, 3, contained_matches, 0) == 0) {
			size_t i = array[idx].contains_len++;
			copy_color(array[idx].contains[i].color, after, &contained_matches[2]);
			array[idx].contains[i].n = atoi(after + contained_matches[1].rm_so);
			after += contained_matches[0].rm_eo;
		}
	}

	// part 1
	for (size_t i=0; i<hashmap_len(&colors); i++) {
		if (can_carry(&array[i], "shiny gold", &colors, array) >= 1) res->part1++;
	}

	// part 2
	size_t idx = *hashmap_lookup(&colors, "shiny gold", 10);
	res->part2 = count_contained(&array[idx], &colors, array);

	regfree(&start_re);
	regfree(&contained_re);
	hashmap_free(&colors);
}

void day_answers_provide(day_answers *answers) {
	*answers = (day_answers) {
		{4, 32},
		{161, 30899},
	};
}
