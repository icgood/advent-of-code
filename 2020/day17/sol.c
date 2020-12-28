#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/hashmap.h"

typedef enum {
	ACTIVE = '#',
	INACTIVE = '.',
} status_t;

typedef struct {
	int x;
	int y;
	int z;
	int w;
} coord_t;

typedef struct {
	coord_t min;
	coord_t max;
} bound_t;

typedef struct {
	status_t status, new_status;
} value_t;

static int count_neighbors(struct hashmap_data *hashmap, value_t *coords, coord_t *coord, int with_w) {
	int x = coord->x, y = coord->y, z = coord->z, w = coord->w;
	int count = 0;
	for (int x2=x-1; x2<=x+1; x2++) {
		for (int y2=y-1; y2<=y+1; y2++) {
			for (int z2=z-1; z2<=z+1; z2++) {
				for (int w2=w-1; w2<=w+1; w2++) {
					if (!with_w && w != 0) continue;
					if (x2 == x && y2 == y && z2 == z && w2 == w) continue;
					coord_t coord2 = {x2, y2, z2, w2};
					size_t *idx = hashmap_lookup(hashmap, &coord2, sizeof (coord_t));
					if (idx != NULL) {
						status_t status = coords[*idx].status;
						if (status == ACTIVE) count++;
					}
				}
			}
		}
	}
	return count;
}

static void cycle_coords(struct hashmap_data *hashmap, value_t **coords, bound_t *bound, int with_w) {
	for (int x=bound->min.x-1; x<=bound->max.x+1; x++) {
		for (int y=bound->min.y-1; y<=bound->max.y+1; y++) {
			for (int z=bound->min.z-1; z<=bound->max.z+1; z++) {
				for (int w=bound->min.w-1; w<=bound->max.w+1; w++) {
					if (!with_w && w != 0) continue;
					coord_t coord = {x, y, z, w};
					int count = count_neighbors(hashmap, *coords, &coord, with_w);
					size_t *idx = hashmap_lookup(hashmap, &coord, sizeof (coord_t));
					value_t *value = idx != NULL ? &(*coords)[*idx] : NULL;
					status_t status = value != NULL ? value->status : INACTIVE;
					if (status == INACTIVE) {
						if (count == 3) {
							if (value == NULL) {
								size_t new_idx = hashmap_add(hashmap, &coord, sizeof (coord_t));
								value = &(*coords)[new_idx];
								value->status = INACTIVE;
							}
							value->new_status = ACTIVE;
						}
					} else if (count != 2 && count != 3) {
						value->new_status = INACTIVE;
					}
				}
			}
		}
	}
}

static void finalize_changes(struct hashmap_key *key, void *value_ptr, size_t idx, void *arg) {
	value_t *value = (value_t *) value_ptr;
	value->status = value->new_status;
}

static void expand_bound(struct hashmap_key *key, void *value_ptr, size_t idx, void *arg) {
	value_t *value = (value_t *) value_ptr;
	if (value->status == ACTIVE) {
		coord_t *coord = (coord_t *) key->buf;
		bound_t *bound = (bound_t *) arg;
		if (coord->x < bound->min.x) bound->min.x = coord->x;
		if (coord->x > bound->max.x) bound->max.x = coord->x;
		if (coord->y < bound->min.y) bound->min.y = coord->y;
		if (coord->y > bound->max.y) bound->max.y = coord->y;
		if (coord->z < bound->min.z) bound->min.z = coord->z;
		if (coord->z > bound->max.z) bound->max.z = coord->z;
		if (coord->w < bound->min.w) bound->min.w = coord->w;
		if (coord->w > bound->max.w) bound->max.w = coord->w;
	}
}

static void count_active(struct hashmap_key *key, void *value_ptr, size_t idx, void *arg) {
	value_t *value = (value_t *) value_ptr;
	day_result_num_t *num = (day_result_num_t *) arg;
	if (value->status == ACTIVE) (*num)++;
}

static void parse_lines(char **lines, struct hashmap_data *hashmap, value_t **coords, bound_t *bound, size_t num_buckets) {
	hashmap_init(hashmap, coords, sizeof (value_t), num_buckets);
	for (size_t y=0; y<32; y++) {
		char *line = lines[y];
		size_t len = strspn(line, ".#");
		for (size_t x=0; x<len; x++) {
			coord_t coord = {x, y, 0, 0};
			size_t idx = hashmap_add(hashmap, &coord, sizeof (coord_t));
			value_t *value = &(*coords)[idx];
			value->status = value->new_status = line[x] == ACTIVE ? ACTIVE : INACTIVE;
		}
	}
	*bound = (bound_t) {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	};
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	char *lines[32];
	struct hashmap_data hashmap;
	value_t *coords;
	bound_t bound;

	for (size_t i=0; i<32; i++) {
		lines[i] = malloc(256);
		if (fgets(lines[i], 256, in) == NULL) {
			lines[i][0] = '\n';
			lines[i][1] = '\0';
		}
	}

	// part 1
	parse_lines(lines, &hashmap, &coords, &bound, 256);
	for (int i=0; i<6; i++) {
		hashmap_foreach(&hashmap, expand_bound, &bound);
		cycle_coords(&hashmap, &coords, &bound, 0);
		hashmap_foreach(&hashmap, finalize_changes, NULL);
	}
	hashmap_foreach(&hashmap, count_active, &res->part1);
	hashmap_free(&hashmap);

	// part 2
	parse_lines(lines, &hashmap, &coords, &bound, 2048);
	for (int i=0; i<6; i++) {
		hashmap_foreach(&hashmap, expand_bound, &bound);
		cycle_coords(&hashmap, &coords, &bound, 1);
		hashmap_foreach(&hashmap, finalize_changes, NULL);
	}
	hashmap_foreach(&hashmap, count_active, &res->part2);
	hashmap_free(&hashmap);

	for (size_t i=0; i<32; i++) {
		free(lines[i]);
	}
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {112, 848}},
		{DAY_INPUT, {301, 2424}},
	};
	return checks;
}
