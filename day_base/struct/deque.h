#ifndef FILE_DEQUE_H_SEEN
#define FILE_DEQUE_H_SEEN

#include <stdlib.h>
#include "array.h"
#include "bitmap.h"

typedef enum { DEQUE_FRONT = 0, DEQUE_LEFT = 0, DEQUE_BACK = 1, DEQUE_RIGHT = 1 } deque_end_t;
typedef enum { DEQUE_NOOP, DEQUE_ROTATE } deque_action_t;
typedef struct deque_data deque_t;
typedef int (*deque_matcher_t)(struct deque_data *data, size_t node, const void *value_ptr, void *arg_ptr);

struct deque_data {
	size_t head;
	size_t len;
	size_t entry_size;
	struct array_data node_array, value_array;
	struct bitmap_data node_bitmap, value_bitmap;
	struct deque_node *nodes;
};

void deque_init(struct deque_data *data, void *array, size_t entry_size);
void deque_move(struct deque_data *data, struct deque_data *dest_data, void *dest_array);
size_t deque_len(struct deque_data *data);
void *deque_pointer(struct deque_data *data);
size_t deque_add(struct deque_data *data, deque_end_t end);
size_t deque_remove(struct deque_data *data, deque_end_t end);
size_t deque_peek(struct deque_data *data, size_t pos, deque_end_t end);
size_t deque_head(struct deque_data *data);
void deque_set_head(struct deque_data *data, size_t node);
void deque_shift(struct deque_data *data, size_t num, size_t node);
int deque_find(struct deque_data *data, size_t *index, deque_end_t end, deque_action_t action, deque_matcher_t matcher, void *arg_ptr);
void deque_rotate(struct deque_data *data, size_t num, deque_end_t end);
void deque_free(struct deque_data *data);

#endif 
