#ifndef FILE_HASHMAP_H_SEEN
#define FILE_HASHMAP_H_SEEN

#include <stdlib.h>
#include "array.h"

struct hashmap_bucket {
	void *key;
	size_t key_len;
	size_t index;
	struct hashmap_bucket *next;
};

struct hashmap_data {
	size_t num_buckets;
	struct hashmap_bucket **buckets;
	struct array_data array_data;
};

void hashmap_init(struct hashmap_data *data, void *array, size_t entry_size, size_t num_buckets);
size_t hashmap_len(struct hashmap_data *data);
void *hashmap_pointer(struct hashmap_data *data);
size_t hashmap_add(struct hashmap_data *data, const void *key, size_t key_len);
size_t *hashmap_lookup(struct hashmap_data *data, const void *key, size_t key_len);
void hashmap_free(struct hashmap_data *data);

#endif
