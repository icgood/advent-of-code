#ifndef FILE_QUEUE_H_SEEN
#define FILE_QUEUE_H_SEEN

#include <stdlib.h>
#include "array.h"
#include "bitmap.h"

struct queue_data {
	struct queue_node *head;
	struct array_data array_data;
	struct bitmap_data bitmap;
};

struct queue_node {
	size_t index;
	struct queue_node *next;
};

void queue_init(struct queue_data *data, void *array, size_t entry_size);
size_t queue_len(struct queue_data *data);
void *queue_pointer(struct queue_data *data);
size_t queue_push(struct queue_data *data);
size_t queue_pop(struct queue_data *data);
size_t queue_peek(struct queue_data *data, size_t pos);
void queue_free(struct queue_data *data);

#endif 
