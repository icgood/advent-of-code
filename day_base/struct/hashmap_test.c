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
	else assert(!"unexpected index");
}

static void each_add(struct hashmap_key *key, void *value, size_t idx, void *arg) {
	size_t end_num = *(size_t *) arg;
	size_t key_num = *(size_t *) key->buf;
	size_t val_num = *(size_t *) value;
	assert(key_num == val_num);
	size_t next_num = val_num + 1;
	if (next_num >= end_num) return;
	struct hashmap_data *data = key->hashmap;
	size_t next_idx = hashmap_add(data, &next_num, sizeof (size_t));
	size_t *array = hashmap_pointer(data);
	array[next_idx] = next_num;
}

static void test_basic_operations() {
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
}

static void test_foreach_add() {
	struct hashmap_data data;
	size_t *array;

	hashmap_init(&data, &array, sizeof (size_t), 32);

	size_t ten = 10;
	size_t idx = hashmap_add(&data, &ten, sizeof (size_t));
	array[idx] = ten;

	size_t arg = 100;
	hashmap_foreach(&data, each_add, &arg);

	assert(90 == hashmap_len(&data));
	for (size_t i=10; i<100; i++) {
		size_t *idx = hashmap_lookup(&data, &i, sizeof (size_t));
		assert(idx != NULL);
		assert(i == array[*idx]);
	}

	hashmap_free(&data);
}

static void test_update_intersect() {
	struct hashmap_data lhs, rhs;
	number_t *val_lhs, *val_rhs;

	hashmap_init(&lhs, &val_lhs, sizeof (int), 16);
	hashmap_init(&rhs, &val_rhs, sizeof (int), 32);

	assign(&lhs, "one", 1);
	assign(&lhs, "two", 2);
	assign(&lhs, "three", 3);
	assign(&rhs, "two", 20);
	assign(&rhs, "three", 30);
	assign(&rhs, "four", 40);

	hashmap_update(&lhs, HASHMAP_INTERSECT, &rhs);
	assert(2 == hashmap_len(&lhs));

	assert(NULL == hashmap_lookup(&lhs, "one", 3));
	assert(20 == val_lhs[*hashmap_lookup(&lhs, "two", 3)]);
	assert(30 == val_lhs[*hashmap_lookup(&lhs, "three", 5)]);
	assert(NULL == hashmap_lookup(&lhs, "four", 4));

	hashmap_free(&lhs);
	hashmap_free(&rhs);
}

static void test_update_union() {
	struct hashmap_data lhs, rhs;
	number_t *val_lhs, *val_rhs;

	hashmap_init(&lhs, &val_lhs, sizeof (int), 16);
	hashmap_init(&rhs, &val_rhs, sizeof (int), 32);

	assign(&lhs, "one", 1);
	assign(&lhs, "two", 2);
	assign(&lhs, "three", 3);
	assign(&rhs, "two", 20);
	assign(&rhs, "three", 30);
	assign(&rhs, "four", 40);

	hashmap_update(&lhs, HASHMAP_UNION, &rhs);
	assert(4 == hashmap_len(&lhs));

	assert(1 == val_lhs[*hashmap_lookup(&lhs, "one", 3)]);
	assert(20 == val_lhs[*hashmap_lookup(&lhs, "two", 3)]);
	assert(30 == val_lhs[*hashmap_lookup(&lhs, "three", 5)]);
	assert(40 == val_lhs[*hashmap_lookup(&lhs, "four", 4)]);

	hashmap_free(&lhs);
	hashmap_free(&rhs);
}

static void test_update_difference() {
	struct hashmap_data lhs, rhs;
	number_t *val_lhs, *val_rhs;

	hashmap_init(&lhs, &val_lhs, sizeof (int), 16);
	hashmap_init(&rhs, &val_rhs, sizeof (int), 32);

	assign(&lhs, "one", 1);
	assign(&lhs, "two", 2);
	assign(&lhs, "three", 3);
	assign(&rhs, "two", 20);
	assign(&rhs, "three", 30);
	assign(&rhs, "four", 40);

	hashmap_update(&lhs, HASHMAP_DIFFERENCE, &rhs);
	assert(1 == hashmap_len(&lhs));

	assert(1 == val_lhs[*hashmap_lookup(&lhs, "one", 3)]);
	assert(NULL == hashmap_lookup(&lhs, "two", 3));
	assert(NULL == hashmap_lookup(&lhs, "three", 5));
	assert(NULL == hashmap_lookup(&lhs, "four", 4));

	hashmap_free(&lhs);
	hashmap_free(&rhs);
}

int main() {
	test_basic_operations();
	test_foreach_add();
	test_update_intersect();
	test_update_union();
	test_update_difference();
	return 0;
}
