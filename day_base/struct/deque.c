#include <assert.h>
#include <stdlib.h>
#include "array.h"
#include "bitmap.h"
#include "deque.h"

struct deque_node {
	size_t index;
	struct deque_node *prev;
	struct deque_node *next;
};

void deque_init(struct deque_data *data, void *array, size_t entry_size) {
	data->head = NULL;
	data->tail = NULL;
	data->len = 0;
	data->entry_size = entry_size;
	array_init(&data->array_data, array, entry_size);
	bitmap_init(&data->bitmap, 0);
}

size_t deque_len(struct deque_data *data) {
	return data->len;
}

void *deque_pointer(struct deque_data *data) {
	return array_pointer(&data->array_data);
}

size_t deque_add(struct deque_data *data, deque_end_t end) {
	size_t index;
	if (bitmap_count(&data->bitmap) < array_len(&data->array_data)) {
		index = bitmap_min(&data->bitmap, 0);
	} else {
		index = array_add(&data->array_data);
		bitmap_resize(&data->bitmap, array_len(&data->array_data));
	}
	bitmap_set(&data->bitmap, index);

	struct deque_node *dest = malloc(sizeof (struct deque_node));
	dest->index = index;
	data->len++;
	if (data->head == NULL) {
		data->head = data->tail = dest;
		dest->prev = NULL;
		dest->next = NULL;
	} else if (end == DEQUE_FRONT) {
		struct deque_node *old_head = data->head;
		data->head = old_head->prev = dest;
		dest->prev = NULL;
		dest->next = old_head;
	} else /* if (end == DEQUE_BACK) */ {
		struct deque_node *old_tail = data->tail;
		data->tail = old_tail->next = dest;
		dest->prev = old_tail;
		dest->next = NULL;
	}
	return index;
}

size_t deque_remove(struct deque_data *data, deque_end_t end) {
	struct deque_node *removed;
	assert(data->head != NULL || data->tail != NULL);
	if (data->head == data->tail) {
		removed = data->head;
		data->head = data->tail = NULL;
	} else if (end == DEQUE_FRONT) {
		removed = data->head;
		data->head = removed->next;
		data->head->prev = NULL;
	} else /* if (end == DEQUE_BACK) */ {
		removed = data->tail;
		data->tail = removed->prev;
		data->tail->next = NULL;
	}
	size_t index = removed->index;
	bitmap_unset(&data->bitmap, index);
	free(removed);
	data->len--;
	return index;
}

size_t deque_peek(struct deque_data *data, size_t pos, deque_end_t end) {
	size_t i = 0;
	struct deque_node *iter;
	if (end == DEQUE_FRONT) {
		iter = data->head;
		while (iter != NULL && i++ < pos) iter = iter->next;
	} else /* if (end == DEQUE_BACK) */ {
		iter = data->tail;
		while (iter != NULL && i++ < pos) iter = iter->prev;
	}
	assert(iter != NULL);
	return iter->index;
}

int deque_find(struct deque_data *data, size_t *index, deque_action_t action, deque_matcher_t matcher, void *arg_ptr) {
	struct deque_node *iter = data->head;
	void *values = deque_pointer(data);
	size_t entry_size = data->entry_size;
	while (iter != NULL) {
		if (matcher(values + (iter->index * entry_size), arg_ptr)) break;
		iter = iter->next;
	}
	if (iter == NULL) return 0;
	if (index != NULL) *index = iter->index;
	if (action != DEQUE_GET) {
		assert(!"action not implemented");
	}
	return 1;
}

void deque_free(struct deque_data *data) {
	struct deque_node *iter = data->head;
	while (iter != NULL) {
		struct deque_node *next = iter->next;
		free(iter);
		iter = next;
	}
	array_free(&data->array_data);
	bitmap_free(&data->bitmap);
}
