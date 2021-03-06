#ifndef FILE_ARRAY_H_SEEN
#define FILE_ARRAY_H_SEEN

#include <stdlib.h>

typedef struct array_data array_t;

struct array_data {
	size_t len;
	size_t size;
	size_t entry_size;
	void **array_ptr;
};

void array_init(struct array_data *data, void *array_ptr, size_t entry_size);
void array_move(struct array_data *data, struct array_data *dest_data, void *dest_array_ptr);
size_t array_len(struct array_data *data);
size_t array_sizeof(struct array_data *data);
void *array_pointer(struct array_data *data);
size_t array_add(struct array_data *data);
void array_realloc(struct array_data *data, size_t size_hint);
void array_resize(struct array_data *data, size_t len);
void array_free(struct array_data *data);

#endif
