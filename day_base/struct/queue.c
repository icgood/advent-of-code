#include <assert.h>
#include <stdlib.h>
#include "array.h"
#include "bitmap.h"
#include "queue.h"

void queue_init(struct queue_data *data, void *array, size_t entry_size) {
	data->head = NULL;
	array_init(&data->array_data, array, entry_size);
	bitmap_init(&data->bitmap, 0);
}

size_t queue_len(struct queue_data *data) {
	return bitmap_count(&data->bitmap);
}

void *queue_pointer(struct queue_data *data) {
	return array_pointer(&data->array_data);
}

static size_t first_empty(struct bitmap_data *bitmap) {
	for (size_t i=0; i<bitmap_len(bitmap); i++) {
		if (!bitmap_get(bitmap, i)) return i;
	}
	assert(0);
}

size_t queue_push(struct queue_data *data) {
	size_t index;
	if (bitmap_count(&data->bitmap) < array_len(&data->array_data)) {
		index = first_empty(&data->bitmap);
	} else {
		index = array_add(&data->array_data);
		bitmap_resize(&data->bitmap, array_len(&data->array_data));
	}
	bitmap_set(&data->bitmap, index);

	struct queue_node **iter = &data->head;
	while (*iter != NULL) iter = &(*iter)->next;
	struct queue_node *dest = *iter = malloc(sizeof (struct queue_node));
	dest->next = NULL;
	dest->index = index;
	return index;
}

size_t queue_pop(struct queue_data *data) {
	struct queue_node *old_head = data->head;
	assert(old_head != NULL);
	size_t index = old_head->index;
	bitmap_unset(&data->bitmap, index);
	data->head = old_head->next;
	free(old_head);
	return index;
}

size_t queue_peek(struct queue_data *data, size_t pos) {
	size_t i = 0;
	struct queue_node *iter = data->head;
	while (iter != NULL && i++ < pos) iter = iter->next;
	assert(iter != NULL);
	return iter->index;
}

void queue_free(struct queue_data *data) {
	struct queue_node *iter = data->head;
	while (iter != NULL) {
		struct queue_node *next = iter->next;
		free(iter);
		iter = next;
	}
	array_free(&data->array_data);
	bitmap_free(&data->bitmap);
}
