#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "hashmap.h"

struct hashmap_bucket_node {
	struct hashmap_bucket_node *next;
	size_t index;
	size_t key_len;
	unsigned char key[];
};

struct hashmap_bucket {
	struct hashmap_bucket_node *head;
	struct hashmap_bucket_node *tail;
	size_t hint;
};

static size_t hash(const char *str, size_t i) {
        if (i == 0) return str[0];
        else return (hash(str, i-1) * 33) ^ str[i];
}

static int keyeq(struct hashmap_key *key_obj, size_t key_hash, const void *key, size_t key_len) {
	return key_obj->hash == key_hash
		&& key_len == key_obj->len
		&& memcmp(key_obj->buf, key, key_len) == 0;
}

void hashmap_init(struct hashmap_data *data, void *array, size_t entry_size, size_t size_hint) {
	data->num_buckets = size_hint;
	data->entry_size = entry_size;
	data->buckets = calloc(data->num_buckets, sizeof (struct hashmap_bucket));
	array_init(&data->key_data, &data->keys, sizeof (struct hashmap_key));
	array_init(&data->value_data, array, entry_size);
	array_realloc(&data->key_data, size_hint);
	array_realloc(&data->value_data, size_hint);
}

size_t hashmap_len(struct hashmap_data *data) {
	return data->value_data.len;
}

void *hashmap_pointer(struct hashmap_data *data) {
	return array_pointer(&data->value_data);
}

size_t hashmap_add(struct hashmap_data *data, const void *key, size_t key_len) {
	size_t key_hash = hash(key, key_len-1);
	size_t bucket_idx = key_hash % data->num_buckets;
	struct hashmap_key *keys = data->keys;
	struct hashmap_bucket *bucket = &data->buckets[bucket_idx];
	struct hashmap_bucket_node *head = bucket->head;
	struct hashmap_bucket_node *tail = bucket->tail;
	struct hashmap_bucket_node **iter;
	if ((bucket->hint & key_hash) != key_hash) {
		iter = tail != NULL ? &tail->next : &head;
	} else {
		iter = &head;
		while (*iter != NULL) {
			struct hashmap_bucket_node *cur = *iter;
			if (keyeq(&keys[cur->index], key_hash, key, key_len)) break;
			iter = &cur->next;
		}
	}
	struct hashmap_bucket_node *dest = *iter;
	if (dest != NULL) {
		return dest->index;
	} else {
		dest = *iter = malloc(sizeof (struct hashmap_bucket_node) + key_len);
		size_t index = array_add(&data->value_data);
		assert(index == array_add(&data->key_data));
		bucket->hint |= key_hash;
		struct hashmap_key *key_obj = &data->keys[index];
		bucket->head = head;
		bucket->tail = dest;
		dest->next = NULL;
		dest->index = index;
		dest->key_len = key_len;
		memcpy(dest->key, key, key_len);
		key_obj->hashmap = data;
		key_obj->hash = key_hash;
		key_obj->buf = dest->key;
		key_obj->len = key_len;
		return index;
	}
}

size_t *hashmap_lookup(struct hashmap_data *data, const void *key, size_t key_len) {
	size_t key_hash = hash(key, key_len-1);
	size_t bucket_idx = key_hash % data->num_buckets;
	struct hashmap_bucket *bucket = &data->buckets[bucket_idx];
	if ((bucket->hint & key_hash) != key_hash) return NULL;
	struct hashmap_bucket_node *iter = bucket->head;
	struct hashmap_key *keys = data->keys;
	while (iter != NULL) {
		if (keyeq(&keys[iter->index], key_hash, key, key_len)) return &iter->index;
		iter = iter->next;
	}
	return NULL;
}

void hashmap_foreach(struct hashmap_data *data, hashmap_foreach_t func, void *arg) {
	struct hashmap_key *keys = data->keys;
	void *value = hashmap_pointer(data);
	size_t len = hashmap_len(data);
	size_t entry_size = data->entry_size;
	for (size_t i=0; i<len; i++) {
		struct hashmap_key *key = &keys[i];
		func(key, value, i, arg);
		value += entry_size;
	}
}

void hashmap_free(struct hashmap_data *data) {
	struct hashmap_bucket *buckets = data->buckets;
	size_t num_buckets = data->num_buckets;
	for (size_t i=0; i<num_buckets; i++) {
		struct hashmap_bucket_node *next, *iter = buckets[i].head;
		while (iter != NULL) {
			next = iter->next;
			free(iter);
			iter = next;
		}
	}
	free(data->buckets);
	array_free(&data->key_data);
	array_free(&data->value_data);
}
