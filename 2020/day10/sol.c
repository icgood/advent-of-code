#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"
#include "day_base/struct/bitmap.h"

static size_t count_ways(struct bitmap_data *bitmap, size_t *cache, size_t pos, size_t goal) {
	if (cache[pos] > 0) {
		return cache[pos];
	}
	if (pos > 0 && !bitmap_get(bitmap, pos)) return 0;
	else if (pos == goal) return 1;
	size_t result = count_ways(bitmap, cache, pos+1, goal)
		+ count_ways(bitmap, cache, pos+2, goal)
		+ count_ways(bitmap, cache, pos+3, goal);
	cache[pos] = result;
	return result;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	struct bitmap_data bitmap;
	size_t num;
	bitmap_init(&bitmap, 0);

	while (fscanf(in, "%lu\n", &num) > 0) {
		bitmap_resize(&bitmap, num);
		bitmap_set(&bitmap, num);
	}

	// part 1
	size_t one_diffs = 0, three_diffs = 1;
	size_t prev_num = 0;
	for (size_t i=0; i<bitmap_len(&bitmap); i++) {
		if (bitmap_get(&bitmap, i)) {
			size_t diff = i - prev_num;
			prev_num = i;
			if (diff == 1) one_diffs++;
			else if (diff == 3) three_diffs++;
		}
	}
	res->part1 = one_diffs * three_diffs;

	// part 2
	size_t goal = bitmap_max(&bitmap, 1);
	struct array_data cache;
	size_t *cache_array;
	array_init(&cache, &cache_array, sizeof (size_t));
	array_resize(&cache, goal+1);
	res->part2 = count_ways(&bitmap, cache_array, 0, goal);
	array_free(&cache);

	bitmap_free(&bitmap);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {220, 19208}},
		{DAY_INPUT, {1820, 3454189699072}},
	};
	return checks;
}
