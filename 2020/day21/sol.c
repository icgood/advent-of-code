#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day_base/struct/bitmap.h"
#include "day_base/struct/hashmap.h"

#define MAX_FOODS 50
#define MAX_ALLERGENS 5

typedef enum { DAIRY, EGGS, FISH, NUTS, PEANUTS, SESAME, SHELLFISH, SOY, WHEAT, NUM_ALLERGENS } allergen_t;
const char * const ALLERGEN_NAMES[NUM_ALLERGENS] = { "dairy", "eggs", "fish", "nuts", "peanuts", "sesame", "shellfish", "soy", "wheat" };

typedef struct {
	hashmap_t ingredients;
	allergen_t allergens[MAX_ALLERGENS];
	size_t num_allergens;
} food_t;

static allergen_t get_allergen(char *from) {
	for (allergen_t i=0; i<NUM_ALLERGENS; i++) {
		size_t allergen_len = strlen(ALLERGEN_NAMES[i]);
		if (strncmp(from, ALLERGEN_NAMES[i], allergen_len) == 0) {
			return i;
		}
	}
	assert(!"invalid allergen name");
}

static size_t parse_lines(FILE *in, food_t *foods, hashmap_t *ingredients) {
	char line[1024];
	size_t idx = 0;
	while (fgets(line, sizeof (line), in) != NULL) {
		food_t *food = &foods[idx++];
		assert(idx <= MAX_FOODS);
		hashmap_init(&food->ingredients, NULL, 0, 128);
		char *ptr = line;
		while (1) {
			ptr += strspn(ptr, " ");
			size_t word_len = strspn(ptr, "abcdefghijklmnopqrstuvwxyz");
			if (word_len == 0) break;
			hashmap_add(ingredients, ptr, word_len);
			hashmap_add(&food->ingredients, ptr, word_len);
			ptr += word_len;
		}
		if (strncmp(ptr, "(contains", 9) == 0) {
			ptr += 9;
			while (1) {
				ptr += strspn(ptr, ", ");
				size_t word_len = strspn(ptr, "abcdefghijklmnopqrstuvwxyz");
				if (word_len == 0) break;
				food->allergens[food->num_allergens++] = get_allergen(ptr);
				assert(food->num_allergens <= MAX_ALLERGENS);
				ptr += word_len;
			}
		}
	}
	return idx;
}

static void determine_allergen_ingredients(hashmap_t *allergens, food_t *foods, size_t foods_len) {
	for (size_t i=0; i<foods_len; i++) {
		food_t *food = &foods[i];
		for (size_t j=0; j<food->num_allergens; j++) {
			allergen_t allergen = food->allergens[j];
			hashmap_op_t op = (hashmap_len(&allergens[allergen]) == 0) ? HASHMAP_UNION : HASHMAP_INTERSECT;
			hashmap_update(&allergens[allergen], op, &food->ingredients);
		}
	}

	bitmap_t determined_allergens;
	hashmap_t determined_ingredients;
	bitmap_init(&determined_allergens, NUM_ALLERGENS);
	hashmap_init(&determined_ingredients, NULL, 0, NUM_ALLERGENS);
	while (bitmap_count(&determined_allergens) < NUM_ALLERGENS) {
		for (allergen_t i=0; i<NUM_ALLERGENS; i++) {
			if (bitmap_get(&determined_allergens, i)) continue;
			size_t remaining = hashmap_len(&allergens[i]);
			if (remaining == 0) {
				bitmap_set(&determined_allergens, i);
			} else if (remaining == 1) {
				bitmap_set(&determined_allergens, i);
				struct hashmap_key *key = &allergens[i].keys[0];
				hashmap_add(&determined_ingredients, key->buf, key->len);
			} else {
				hashmap_update(&allergens[i], HASHMAP_DIFFERENCE, &determined_ingredients);
			}
		}
	}
	bitmap_free(&determined_allergens);
	hashmap_free(&determined_ingredients);
}

void day_result_compute(char *arg, day_result *res, FILE *in) {
	food_t foods[MAX_FOODS] = {};
	hashmap_t ingredients;
	hashmap_t allergens[NUM_ALLERGENS];

	hashmap_init(&ingredients, NULL, 0, 256);
	for (allergen_t i=0; i<NUM_ALLERGENS; i++) hashmap_init(&allergens[i], NULL, 0, 64);
	size_t foods_len = parse_lines(in, foods, &ingredients);
	determine_allergen_ingredients(allergens, foods, foods_len);

	// part 1
	hashmap_t allergen_ingredients;
	hashmap_init(&allergen_ingredients, NULL, 0, NUM_ALLERGENS);
	for (allergen_t allergen=0; allergen<NUM_ALLERGENS; allergen++) {
		hashmap_update(&allergen_ingredients, HASHMAP_UNION, &allergens[allergen]);
	}
	for (size_t i=0; i<foods_len; i++) {
		food_t *food = &foods[i];
		hashmap_update(&food->ingredients, HASHMAP_DIFFERENCE, &allergen_ingredients);
		res->part1 += hashmap_len(&food->ingredients);
	}
	hashmap_free(&allergen_ingredients);

	// part 2
	for (allergen_t i=0; i<NUM_ALLERGENS; i++) {
		if (hashmap_len(&allergens[i]) == 0) continue;
		struct hashmap_key *key = &allergens[i].keys[0];
		sprintf(res->part2_str + strlen(res->part2_str), "%.*s,", (int) key->len, (char *) key->buf);
	}
	res->part2_str[strlen(res->part2_str) - 1] = '\0';

	for (allergen_t i=0; i<NUM_ALLERGENS; i++) hashmap_free(&allergens[i]);
	for (size_t i=0; i<foods_len; i++) hashmap_free(&foods[i].ingredients);
	hashmap_free(&ingredients);
}

void day_answers_provide(day_arguments *args, day_answers *answers) {
	*answers = (day_answers) {
		{5, 0, "", "mxmxvkd,sqjhc,fvjkl"},
		{2317, 0, "", "kbdgs,sqvv,slkfgq,vgnj,brdd,tpd,csfmb,lrnz"},
	};
}
