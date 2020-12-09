#ifndef FILE_BITMAP_H_SEEN
#define FILE_BITMAP_H_SEEN

#include <stdlib.h>

struct bitmap_data {
	unsigned char *bitmap_ptr;
	size_t size;
};

void bitmap_init(struct bitmap_data *data, size_t len);
size_t bitmap_len(struct bitmap_data *data);
unsigned char *bitmap_pointer(struct bitmap_data *data);
size_t bitmap_count(struct bitmap_data *data);
void bitmap_set(struct bitmap_data *data, size_t pos);
void bitmap_unset(struct bitmap_data *data, size_t pos);
int bitmap_get(struct bitmap_data *data, size_t pos);
void bitmap_resize(struct bitmap_data *data, size_t len);
void bitmap_free(struct bitmap_data *data);

#endif
