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

void hashmap_move(struct hashmap_data *data, struct hashmap_data *dest_data, void *dest_array) {
	if (data != dest_data) *dest_data = *data;
	array_move(&data->key_data, &dest_data->key_data, &dest_data->keys);
	array_move(&data->value_data, &dest_data->value_data, dest_array);
	for (size_t i=0; i<data->num_buckets; i++) {
		deque_move(&data->buckets[i].deque, &dest_data->buckets[i].deque, &dest_data->buckets[i].indexes);
	}
}

size_t hashmap_len(struct hashmap_data *data) {
	return array_len(&data->key_data);
}

void *hashmap_pointer(struct hashmap_data *data) {
	return array_pointer(&data->value_data);
}

static int key_matcher(struct deque_data *data, size_t node, const void *value_ptr, void *arg_ptr) {
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
		if (deque_find(&bucket->deque, &sub_idx, DEQUE_LEFT, 0, key_matcher, &arg)) {
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
	if (deque_find(&bucket->deque, &sub_idx, DEQUE_LEFT, 0, key_matcher, &arg)) {
		return &bucket->indexes[sub_idx];
	}
	return NULL;
}

void hashmap_update(struct hashmap_data *data, hashmap_op_t op, struct hashmap_data *rhs) {
	size_t entry_size = data->entry_size;
	assert(entry_size == rhs->entry_size);
	struct hashmap_data lhs;
	void *lhs_array, **values = data->value_data.array_ptr;
	hashmap_move(data, &lhs, &lhs_array);
	hashmap_init(data, values, entry_size, data->num_buckets);
	struct hashmap_key *lhs_keys = lhs.keys, *rhs_keys = rhs->keys;
	size_t lhs_len = array_len(&lhs.key_data), rhs_len = array_len(&rhs->key_data);
	void *lhs_ptr = entry_size > 0 ? hashmap_pointer(&lhs) : NULL;
	void *rhs_ptr = entry_size > 0 ? hashmap_pointer(rhs) : NULL;
	if (op == HASHMAP_UNION) {
		for (size_t i=0; i<lhs_len; i++) {
			struct hashmap_key *key = &lhs_keys[i];
			size_t idx = hashmap_add(data, key->buf, key->len);
			if (entry_size > 0) {
				void *lhs_value = lhs_ptr + (i * entry_size);
				memcpy(*values + (idx * entry_size), lhs_value, entry_size);
			}
		}
		for (size_t i=0; i<rhs_len; i++) {
			struct hashmap_key *key = &rhs_keys[i];
			size_t idx = hashmap_add(data, key->buf, key->len);
			if (entry_size > 0) {
				void *rhs_value = rhs_ptr + (i * entry_size);
				memcpy(*values + (idx * entry_size), rhs_value, entry_size);
			}
		}
	} else if (op == HASHMAP_INTERSECT) {
		for (size_t i=0; i<rhs_len; i++) {
			struct hashmap_key *key = &rhs_keys[i];
			size_t *lhs_idx = hashmap_lookup(&lhs, key->buf, key->len);
			if (lhs_idx != NULL) {
				size_t idx = hashmap_add(data, key->buf, key->len);
				if (entry_size > 0) {
					void *rhs_value = rhs_ptr + (i * entry_size);
					memcpy(*values + (idx * entry_size), rhs_value, entry_size);
				}
			}
		}
	} else if (op == HASHMAP_DIFFERENCE) {
		for (size_t i=0; i<lhs_len; i++) {
			struct hashmap_key *key = &lhs_keys[i];
			size_t *rhs_idx = hashmap_lookup(rhs, key->buf, key->len);
			if (rhs_idx == NULL) {
				size_t idx = hashmap_add(data, key->buf, key->len);
				if (entry_size > 0) {
					void *lhs_value = lhs_ptr + (i * entry_size);
					memcpy(*values + (idx * entry_size), lhs_value, entry_size);
				}
			}
		}
	} else {
		assert(!"invalid operation");
	}
	hashmap_free(&lhs);
}

void hashmap_foreach(struct hashmap_data *data, hashmap_foreach_t func, void *arg_ptr) {
	size_t entry_size = data->entry_size;
	for (size_t i=0; ; i++) {
		size_t len = hashmap_len(data);
		if (i >= len) break;
		struct hashmap_key *keys = data->keys;
		struct hashmap_key *key = &keys[i];
		void *value = hashmap_pointer(data) + (i * entry_size);
		func(key, value, i, arg_ptr);
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
