#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

typedef struct {
	uint8_t one;
	uint8_t two;
	char letter;
	char password[256];
} entry;

static entry *new_array(size_t new_size, entry *old_array, size_t len) {
	entry *new_array = malloc(new_size * sizeof (entry));
	if (len > 0) memcpy(new_array, old_array, len * sizeof (entry));
	free (old_array);
	return new_array;
}

void day_result_compute(day_result *res, FILE *in) {
	size_t len = 0, size = 2;
	entry *array = new_array(size, NULL, 0);
	char line[1024];

	while (fgets(line, sizeof (line), in) != NULL) {
		sscanf(line, "%hhu-%hhu %c: %s\n", &array[len].one, &array[len].two, &array[len].letter, array[len].password);
		len++;
		if (len >= size) {
			array = new_array(size *= 2, array, len);
		}
	}

	// part 1
	for (size_t i=0; i<len; i++) {
		entry *e = &array[i];
		size_t password_len = strlen(e->password);
		uint8_t count = 0;
		for (size_t j=0; j<password_len; j++) {
			if (e->password[j] == e->letter) count++;
		}
		if (count >= e->one && count <= e->two) res->part1++;
	}

	// part 2
	for (size_t i=0; i<len; i++) {
		entry *e = &array[i];
		int first = e->password[e->one-1] == e->letter;
		int second = e->password[e->two-1] == e->letter;
		if (first ^ second) res->part2++;
	}

	free(array);
}

void day_result_test(char *dir) {
	day_result_check(dir, "example", 2, 1);
	day_result_check(dir, "input", 572, 306);
}
