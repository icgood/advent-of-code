#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"

typedef unsigned int number_t;

static void assign(struct hashmap_data *data, const char *key, uint64_t num) {
	size_t idx = hashmap_add(data, key, strlen(key));
	number_t *array = hashmap_pointer(data);
	array[idx] = num;
}

static void assert_entry(struct hashmap_key *key, const char *key_str, int correct_num) {
	size_t len = strlen(key_str);
	assert(len == key->len);
	assert(memcmp(key->buf, key_str, len) == 0);
	assert(correct_num);
}

static void assert_each(struct hashmap_key *key, void *value, size_t idx, void *arg) {
	*(number_t *) arg |= 1 << idx;
	number_t num = *(number_t *) value;
	if (idx == 0) assert_entry(key, "zero", num == 0);
	else if (idx == 1) assert_entry(key, "one", num == 10);
	else if (idx == 2) assert_entry(key, "two", num == 20);
	else if (idx == 3) assert_entry(key, "three", num == 30);
	else if (idx == 4) assert_entry(key, "four", num == 40);
	else if (idx == 5) assert_entry(key, "five", num == 50);
	else if (idx == 6) assert_entry(key, "six", num == 60);
	else if (idx == 7) assert_entry(key, "seven", num == 70);
	else if (idx == 8) assert_entry(key, "eight", num == 80);
	else if (idx == 9) assert_entry(key, "nine", num == 90);
	else assert(0);
}

int main() {
	struct hashmap_data data;
	number_t *array;

	hashmap_init(&data, &array, sizeof (number_t), 32);

	assign(&data, "zero", 0);
	assign(&data, "one", 0);
	assign(&data, "two", 0);
	assign(&data, "three", 0);

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

	assert(10 == hashmap_len(&data));
	assert(70 == array[7]);
	assert(7 == *hashmap_lookup(&data, "seven", 5));
	assert(NULL == hashmap_lookup(&data, "invalid", 7));

	number_t arg = 0;
	hashmap_foreach(&data, assert_each, &arg);
	assert(1023 == arg);

	hashmap_free(&data);
	return 0;
}
