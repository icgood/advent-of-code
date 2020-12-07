#include <assert.h>
#include <regex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

#define COLOR_MAX 64
#define CONTAINS_MAX 16
#define NUM_BUCKETS 1024 

struct bucket {
	char color[COLOR_MAX];
	size_t idx;
	struct bucket *next;
};

static struct bucket *add_color_to(struct bucket *head, char *color, size_t idx) {
	struct bucket **iter = &head;
	while (*iter != NULL) iter = &(*iter)->next;
	struct bucket *dest = *iter = malloc(sizeof (struct bucket));
	dest->next = NULL;
	strncpy(dest->color, color, 32);
	dest->idx = idx;
	return head;
}

static size_t hash(char *str, size_t i) {
	if (i == 0) return str[0];
	else return (hash(str, i-1) * 33) ^ str[i];
}

static void add_color(struct bucket **buckets, char *color, size_t idx) {
	size_t color_hash = hash(color, strlen(color));
	size_t bucket_idx = color_hash % NUM_BUCKETS;
	struct bucket *head = buckets[bucket_idx];
	buckets[bucket_idx] = add_color_to(head, color, idx);
}

static size_t lookup_color(struct bucket **buckets, char *color) {
	size_t color_hash = hash(color, strlen(color));
	size_t bucket_idx = color_hash % NUM_BUCKETS;
	struct bucket *iter = buckets[bucket_idx];
	while (iter != NULL) {
		if (strcmp(iter->color, color) == 0) return iter->idx;
		iter = iter->next;
	}
	assert(0);
}

static void free_buckets(struct bucket **buckets) {
	for (size_t i=0; i<NUM_BUCKETS; i++) {
		struct bucket *next, *iter = buckets[i];
		while (iter != NULL) {
			next = iter->next;
			free(iter);
			iter = next;
		}
	}
}

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

static color *new_array(size_t new_size, color *old_array, size_t len) {
	color *new_array = calloc(new_size, sizeof (color));
	if (len > 0) memcpy(new_array, old_array, len * sizeof (color));
	free(old_array);
	return new_array;
}

static int can_carry(color *c, char *wanted, struct bucket **buckets, color *array) {
	int total = 0;
	for (size_t i=0; i<c->contains_len; i++) {
		size_t idx = lookup_color(buckets, c->contains[i].color);
		color *contained = &array[idx];
		if (strcmp(contained->color, wanted) == 0) total += c->contains[i].n;
		else total += can_carry(contained, wanted, buckets, array);
	}
	return total;
}

static int count_contained(color *c, struct bucket **buckets, color *array) {
	int total = 0;
	for (size_t i=0; i<c->contains_len; i++) {
		size_t idx = lookup_color(buckets, c->contains[i].color);
		color *contained = &array[idx];
		total += c->contains[i].n + c->contains[i].n * count_contained(contained, buckets, array);
	}
	return total;
}

void day_result_compute(day_result *res, FILE *in) {
	char line[1024];
	size_t len = 0, size = 2;
	color *array = new_array(size, NULL, 0);
	struct bucket *buckets[NUM_BUCKETS];
	memset(buckets, 0, NUM_BUCKETS * sizeof (struct bucket *));

	regex_t start_re, contained_re;
	regmatch_t start_matches[2], contained_matches[3];
	assert(regcomp(&start_re, "^([a-z]+ [a-z]+) bags contain", REG_EXTENDED) == 0);
	assert(regcomp(&contained_re, " ([0-9]+) ([a-z]+ [a-z]+) bags?[,.]", REG_EXTENDED) == 0);

	while (fgets(line, sizeof (line), in) != NULL) {
		if (regexec(&start_re, line, 2, start_matches, 0) != 0) break;

		copy_color(array[len].color, line, &start_matches[1]);
		char *after = line + start_matches[0].rm_eo;
		while (regexec(&contained_re, after, 3, contained_matches, 0) == 0) {
			size_t i = array[len].contains_len++;
			copy_color(array[len].contains[i].color, after, &contained_matches[2]);
			array[len].contains[i].n = atoi(after + contained_matches[1].rm_so);
			after += contained_matches[0].rm_eo;
		}

		add_color(buckets, array[len].color, len);
		if (++len >= size) {
			array = new_array(size *= 2, array, len);
		}
	}

	// part 1
	for (size_t i=0; i<len; i++) {
		if (can_carry(&array[i], "shiny gold", buckets, array) >= 1) res->part1++;
	}

	// part 2
	size_t idx = lookup_color(buckets, "shiny gold");
	color *shiny_gold = &array[idx];
	res->part2 = count_contained(shiny_gold, buckets, array);

	free_buckets(buckets);
	regfree(&start_re);
	regfree(&contained_re);
	free(array);
}

void day_answers_provide(day_answers *answers) {
	*answers = (day_answers) {
		{4, 32},
		{161, 30899},
	};
}
