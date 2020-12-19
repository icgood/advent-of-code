#ifndef FILE_DEQUE_H_SEEN
#define FILE_DEQUE_H_SEEN

#include <stdlib.h>
#include "array.h"
#include "bitmap.h"

typedef enum { DEQUE_FRONT, DEQUE_BACK } deque_end_t;

struct deque_data {
	struct deque_node *head;
	struct deque_node *tail;
	size_t len;
	struct array_data array_data;
	struct bitmap_data bitmap;
};

struct deque_node {
	size_t index;
	struct deque_node *prev;
	struct deque_node *next;
};

void deque_init(struct deque_data *data, void *array, size_t entry_size);
size_t deque_len(struct deque_data *data);
void *deque_pointer(struct deque_data *data);
size_t deque_add(struct deque_data *data, deque_end_t end);
size_t deque_remove(struct deque_data *data, deque_end_t end);
size_t deque_peek(struct deque_data *data, size_t pos, deque_end_t end);
void deque_free(struct deque_data *data);

#endif 
