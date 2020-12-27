#ifndef FILE_HASHMAP_H_SEEN
#define FILE_HASHMAP_H_SEEN

#include <stdlib.h>
#include "array.h"

typedef struct hashmap_key hashmap_key_t;
typedef struct hashmap_data hashmap_t;
typedef enum { HASHMAP_INTERSECT, HASHMAP_UNION, HASHMAP_DIFFERENCE } hashmap_op_t;
typedef void (*hashmap_foreach_t)(struct hashmap_key *key, void *value_ptr, size_t idx, void *arg_ptr);

struct hashmap_key {
	struct hashmap_data *hashmap;
	void *buf;
	size_t len;
	size_t hash;
};

struct hashmap_data {
	size_t num_buckets;
	size_t entry_size;
	struct hashmap_bucket *buckets;
	struct array_data key_data;
	struct array_data value_data;
	struct hashmap_key *keys;
};

void hashmap_init(struct hashmap_data *data, void *array, size_t entry_size, size_t size_hint);
void hashmap_move(struct hashmap_data *data, struct hashmap_data *dest_data, void *dest_array);
size_t hashmap_len(struct hashmap_data *data);
void *hashmap_pointer(struct hashmap_data *data);
size_t hashmap_add(struct hashmap_data *data, const void *key, size_t key_len);
size_t *hashmap_lookup(struct hashmap_data *data, const void *key, size_t key_len);
void hashmap_update(struct hashmap_data *data, hashmap_op_t op, struct hashmap_data *operand);
void hashmap_foreach(struct hashmap_data *data, hashmap_foreach_t func, void *arg_ptr);
void hashmap_free(struct hashmap_data *data);

#endif
