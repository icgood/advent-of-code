#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "bitmap.h"
#include "deque.h"

struct deque_node {
	size_t node_index;
	size_t value_index;
	size_t next;
	size_t prev;
};

void deque_init(struct deque_data *data, void *array, size_t entry_size) {
	data->head = 0;
	data->len = 0;
	data->entry_size = entry_size;
	array_init(&data->node_array, &data->nodes, sizeof (struct deque_node));
	array_init(&data->value_array, array, entry_size);
	bitmap_init(&data->node_bitmap, 0);
	bitmap_init(&data->value_bitmap, 0);
}

void deque_move(struct deque_data *data, struct deque_data *dest_data, void *dest_array) {
	if (data != dest_data) *dest_data = *data;
	array_move(&data->node_array, &dest_data->node_array, &dest_data->nodes);
	array_move(&data->value_array, &dest_data->value_array, dest_array);
}

size_t deque_len(struct deque_data *data) {
	return data->len;
}

void *deque_pointer(struct deque_data *data) {
	return array_pointer(&data->value_array);
}

static size_t find_index(struct array_data *array, struct bitmap_data *bitmap) {
	size_t len = array_len(array);
	size_t index;
	if (bitmap_count(bitmap) < len) {
		index = bitmap_min(bitmap, 0);
	} else {
		index = array_add(array);
		bitmap_resize(bitmap, len+1);
	}
	bitmap_set(bitmap, index);
	return index;
}

size_t deque_add(struct deque_data *data, deque_end_t end) {
	size_t node_index = find_index(&data->node_array, &data->node_bitmap);
	size_t value_index = find_index(&data->value_array, &data->value_bitmap);
	struct deque_node *nodes = data->nodes;
	struct deque_node *node = &nodes[node_index];
	node->node_index = node_index;
	node->value_index = value_index;
	if (data->len == 0) {
		data->head = node->next = node->prev = node_index;
	} else {
		struct deque_node *head_node = &nodes[data->head];
		struct deque_node *head_prev = &nodes[head_node->prev];
		node->prev = head_prev->node_index;
		node->next = head_node->node_index;
		head_prev->next = node_index;
		head_node->prev = node_index;
		if (end == DEQUE_FRONT) data->head = node_index;
	}
	data->len++;
	return value_index;
}

size_t deque_remove(struct deque_data *data, deque_end_t end) {
	assert(data->len != 0 || !"deque is empty");
	struct deque_node *nodes = data->nodes;
	struct deque_node *remove;
	if (data->len == 1) {
		remove = &nodes[data->head];
	} else {
		struct deque_node *head_node = &nodes[data->head];
		remove = (end == DEQUE_FRONT) ? head_node : &nodes[head_node->prev];
		struct deque_node *remove_prev = &nodes[remove->prev];
		struct deque_node *remove_next = &nodes[remove->next];
		remove_prev->next = remove->next;
		remove_next->prev = remove->prev;
		if (end == DEQUE_FRONT) data->head = remove->next;
	}
	data->len--;
	bitmap_unset(&data->node_bitmap, remove->node_index);
	bitmap_unset(&data->value_bitmap, remove->value_index);
	return remove->value_index;
}

size_t deque_peek(struct deque_data *data, size_t pos, deque_end_t end) {
	size_t len = deque_len(data);
	assert(pos < len || !"position out of bounds");
	struct deque_node *nodes = data->nodes;
	struct deque_node *node = &nodes[data->head];
	if (end == DEQUE_FRONT) {
		for (size_t i=0; i<pos; i++) node = &nodes[node->next];
	} else /* if (end == DEQUE_BACK) */ {
		for (size_t i=0; i<=pos; i++) node = &nodes[node->prev];
	}
	return node->value_index;
}

size_t deque_head(struct deque_data *data) {
	return data->head;
}

void deque_set_head(struct deque_data *data, size_t node) {
	data->head = node;
}

void deque_shift(struct deque_data *data, size_t num, size_t node) {
	size_t len = deque_len(data);
	assert(len >= num || !"not enough elements to shift");
	assert(node < len || !"invalid node");
	struct deque_node *nodes = data->nodes;
	struct deque_node *dest = &nodes[node];
	struct deque_node *start = &nodes[data->head], *end = start;
	for (size_t i=1; i<num; i++) end = &nodes[end->next];
	nodes[start->prev].next = data->head = end->next;
	nodes[end->next].prev = start->prev;
	nodes[dest->prev].next = start->node_index;
	start->prev = dest->prev;
	dest->prev = end->node_index;
	end->next = dest->node_index;
}

int deque_find(struct deque_data *data, size_t *index, deque_end_t end, deque_action_t action, deque_matcher_t matcher, void *arg_ptr) {
	size_t len = deque_len(data);
	if (len <= 0) return 0;
	size_t entry_size = data->entry_size;
	void *values = deque_pointer(data);
	struct deque_node *nodes = data->nodes;
	struct deque_node *node = &nodes[data->head];
	int matched = 0;
	for (size_t i=0; i<len; i++) {
		if (matcher == NULL || matcher(data, node->node_index, values + (node->value_index * entry_size), arg_ptr)) {
			matched = 1;
			break;
		}
		node = &nodes[end == DEQUE_FRONT ? node->next : node->prev];
	}
	if (matched) {
		if (index != NULL) *index = node->value_index;
		if (action == DEQUE_ROTATE) {
			data->head = node->node_index;
		} else if (action != DEQUE_NOOP) {
			assert(!"invalid action");
		}
	}
	return matched;
}

void deque_rotate(struct deque_data *data, size_t num, deque_end_t end) {
	size_t len = deque_len(data);
	num %= len;
	struct deque_node *nodes = data->nodes;
	struct deque_node *head = &nodes[data->head];
	for (size_t i=0; i<num; i++) {
		head = &nodes[end == DEQUE_FRONT ? head->next : head->prev];
	}
	data->head = head->node_index;
}

void deque_free(struct deque_data *data) {
	array_free(&data->node_array);
	array_free(&data->value_array);
	bitmap_free(&data->node_bitmap);
	bitmap_free(&data->value_bitmap);
}
