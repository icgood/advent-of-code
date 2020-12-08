#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"

typedef struct {
	const char *key;
	int64_t num;
} test;

static void assign(struct hashmap_data *data, const char *key, uint64_t num) {
	size_t idx = hashmap_add(data, key, strlen(key));
	test *array = hashmap_pointer(data);
	array[idx].key = key;
	array[idx].num = num;
}

int main() {
	struct hashmap_data data;
	test *array;

	hashmap_init(&data, &array, sizeof (test), 32);

	assign(&data, "zero", 0);
	assign(&data, "one", 10);
	assign(&data, "two", 20);
	assign(&data, "three", 30);
	assign(&data, "four", 40);
	assign(&data, "five", 50);
	assign(&data, "six", 60);
	assign(&data, "seven", 70);
	assign(&data, "eight", 80);
	assign(&data, "nine", 90);

	assert(0 == strcmp("seven", array[7].key));
	assert(70 == array[7].num);
	assert(7 == *hashmap_lookup(&data, "seven", 5));
	assert(NULL == hashmap_lookup(&data, "invalid", 7));

	int64_t total = 0;
	total += array[*hashmap_lookup(&data, "three", 5)].num;
	total *= array[*hashmap_lookup(&data, "nine", 4)].num;
	total /= array[*hashmap_lookup(&data, "one", 3)].num;
	total -= array[*hashmap_lookup(&data, "two", 3)].num;
	total /= array[*hashmap_lookup(&data, "five", 4)].num;
	assert(5 == total);

	hashmap_free(&data);
	return 0;
}
