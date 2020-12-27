#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "deque.h"
#include "hashmap.h"

struct hashmap_bucket {
	struct deque_data deque;
	size_t *indexes;
	size_t hint;
};

struct hashmap_matcher_arg {
	struct hashmap_key *keys;
	const void *key;
	size_t key_len;
	size_t key_hash;
};

static size_t hash(const char *str, size_t i) {
        if (i == 0) return str[0];
        else return (hash(str, i-1) * 33) ^ str[i];
}

void hashmap_init(struct hashmap_data *data, void *array, size_t entry_size, size_t size_hint) {
	data->num_buckets = size_hint;
	data->entry_size = entry_size;
	struct hashmap_bucket *buckets = data->buckets = calloc(data->num_buckets, sizeof (struct hashmap_bucket));
	for (size_t i=0; i<data->num_buckets; i++) {
		struct hashmap_bucket *bucket = &buckets[i];
		deque_init(&bucket->deque, &bucket->indexes, sizeof (size_t));
	}
	array_init(&data->key_data, &data->keys, sizeof (struct hashmap_key));
	array_init(&data->value_data, array, entry_size);
	array_realloc(&data->key_data, size_hint);
	array_realloc(&data->value_data, size_hint);
}

size_t hashmap_len(struct hashmap_data *data) {
	return array_len(&data->key_data);
}

void *hashmap_pointer(struct hashmap_data *data) {
	return array_pointer(&data->value_data);
}

static int key_matcher(void *value_ptr, void *arg_ptr) {
	struct hashmap_matcher_arg *arg = (struct hashmap_matcher_arg *) arg_ptr;
	size_t index = *(size_t *) value_ptr;
	struct hashmap_key *key = &arg->keys[index];
	return arg->key_hash == key->hash
		&& arg->key_len == key->len
		&& memcmp(arg->key, key->buf, key->len) == 0;
}

size_t hashmap_add(struct hashmap_data *data, const void *key, size_t key_len) {
	size_t key_hash = hash(key, key_len-1);
	size_t bucket_idx = key_hash % data->num_buckets;
	struct hashmap_bucket *bucket = &data->buckets[bucket_idx];
	if ((bucket->hint & key_hash) == key_hash) {
		struct hashmap_matcher_arg arg = {data->keys, key, key_len, key_hash};
		size_t sub_idx;
		if (deque_find(&bucket->deque, &sub_idx, 0, key_matcher, &arg)) {
			return bucket->indexes[sub_idx];
		}
	}
	size_t sub_idx = deque_add(&bucket->deque, DEQUE_FRONT);
	size_t index = bucket->indexes[sub_idx] = array_add(&data->value_data);
	assert(index == array_add(&data->key_data));
	bucket->hint |= key_hash;
	struct hashmap_key *key_obj = &data->keys[index];
	key_obj->hashmap = data;
	key_obj->hash = key_hash;
	key_obj->len = key_len;
	key_obj->buf = malloc(key_len);;
	memcpy(key_obj->buf, key, key_len);
	return index;
}

size_t *hashmap_lookup(struct hashmap_data *data, const void *key, size_t key_len) {
	size_t key_hash = hash(key, key_len-1);
	size_t bucket_idx = key_hash % data->num_buckets;
	struct hashmap_bucket *bucket = &data->buckets[bucket_idx];
	if ((bucket->hint & key_hash) != key_hash) return NULL;
	struct hashmap_matcher_arg arg = {data->keys, key, key_len, key_hash};
	size_t sub_idx;
	if (deque_find(&bucket->deque, &sub_idx, 0, key_matcher, &arg)) {
		return &bucket->indexes[sub_idx];
	}
	return NULL;
}

void hashmap_join(struct hashmap_data *data, hashmap_join_t join, struct hashmap_data *one, struct hashmap_data *two) {
	void *values = hashmap_pointer(data);
	size_t entry_size = data->entry_size;
	assert(one->entry_size == entry_size && two->entry_size == entry_size);
	if (join == HASHMAP_INTERSECT) {
		struct hashmap_key *one_keys = one->keys;
		size_t one_len = array_len(&one->key_data);
		void *two_values = hashmap_pointer(two);
		for (size_t i=0; i<one_len; i++) {
			struct hashmap_key *key = &one_keys[i];
			size_t *two_idx = hashmap_lookup(two, key->buf, key->len);
			if (two_idx != NULL) {
				void *two_value = two_values + (*two_idx * entry_size);
				size_t idx = hashmap_add(data, key->buf, key->len);
				memcpy(values + (idx * entry_size), two_value, entry_size);
			}
		}
	} else {
		assert(!"invalid join type");
	}
}

void hashmap_foreach(struct hashmap_data *data, hashmap_foreach_t func, void *arg_ptr) {
	struct hashmap_key *keys = data->keys;
	void *value = hashmap_pointer(data);
	size_t len = hashmap_len(data);
	size_t entry_size = data->entry_size;
	for (size_t i=0; i<len; i++) {
		struct hashmap_key *key = &keys[i];
		func(key, value, i, arg_ptr);
		value += entry_size;
	}
}

void hashmap_free(struct hashmap_data *data) {
	struct hashmap_bucket *buckets = data->buckets;
	size_t num_buckets = data->num_buckets;
	for (size_t i=0; i<num_buckets; i++) deque_free(&buckets[i].deque);
	for (size_t i=0; i<array_len(&data->key_data); i++) free(data->keys[i].buf);
	free(data->buckets);
	array_free(&data->key_data);
	array_free(&data->value_data);
}
