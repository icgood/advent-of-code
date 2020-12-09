#ifndef FILE_ARRAY_H_SEEN
#define FILE_ARRAY_H_SEEN

#include <stdlib.h>

struct array_data {
	size_t len;
	size_t size;
	size_t entry_size;
	void **array_ptr;
};

void array_init(struct array_data *data, void *array_ptr, size_t entry_size);
size_t array_len(struct array_data *data);
void *array_pointer(struct array_data *data);
size_t array_add(struct array_data *data);
void array_free(struct array_data *data);

#endif
