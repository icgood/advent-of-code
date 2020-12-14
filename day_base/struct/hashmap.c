#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "hashmap.h"

static size_t hash(const char *str, size_t i) {
        if (i == 0) return str[0];
        else return (hash(str, i-1) * 33) ^ str[i];
}

void hashmap_init(struct hashmap_data *data, void *array, size_t entry_size, size_t num_buckets) {
	data->num_buckets = num_buckets;
	data->buckets = malloc(num_buckets * sizeof (struct hashmap_bucket *));
	memset(data->buckets, 0, num_buckets * sizeof (struct hashmap_bucket *));
	array_init(&data->array_data, array, entry_size);
}

size_t hashmap_len(struct hashmap_data *data) {
	return data->array_data.len;
}

void *hashmap_pointer(struct hashmap_data *data) {
	return array_pointer(&data->array_data);
}

size_t hashmap_add(struct hashmap_data *data, const void *key, size_t key_len) {
	size_t key_hash = hash(key, key_len-1);
	size_t bucket_idx = key_hash % data->num_buckets;
	struct hashmap_bucket *head = data->buckets[bucket_idx];
	struct hashmap_bucket **iter = &head;
	while (*iter != NULL) {
		struct hashmap_bucket *cur = *iter;
		if (cur->key_len == key_len && memcmp(cur->key, key, key_len) == 0) break;
		iter = &cur->next;
	}
	struct hashmap_bucket *dest = *iter;
	if (dest != NULL) {
		return dest->index;
	} else {
		dest = *iter = malloc(sizeof (struct hashmap_bucket));
		size_t index = array_add(&data->array_data);
		data->buckets[bucket_idx] = head;
		dest->next = NULL;
		dest->index = index;
		dest->key_len = key_len;
		char *key_copy = dest->key = malloc(key_len);
		memcpy(key_copy, key, key_len);
		return index;
	}
}

size_t *hashmap_lookup(struct hashmap_data *data, const void *key, size_t key_len) {
	size_t key_hash = hash(key, key_len-1);
	size_t bucket_idx = key_hash % data->num_buckets;
	struct hashmap_bucket *iter = data->buckets[bucket_idx];
	while (iter != NULL) {
		if (iter->key_len == key_len && memcmp(iter->key, key, key_len) == 0) return &iter->index;
		iter = iter->next;
	}
	return NULL;
}

void hashmap_free(struct hashmap_data *data) {
	struct hashmap_bucket **buckets = data->buckets;
	size_t num_buckets = data->num_buckets;
	for (size_t i=0; i<num_buckets; i++) {
		struct hashmap_bucket *next, *iter = buckets[i];
		while (iter != NULL) {
			next = iter->next;
			free(iter->key);
			free(iter);
			iter = next;
		}
	}
	free(data->buckets);
	array_free(&data->array_data);
}
