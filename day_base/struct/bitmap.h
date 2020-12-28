#ifndef FILE_BITMAP_H_SEEN
#define FILE_BITMAP_H_SEEN

#include <stdlib.h>

typedef enum { BITMAP_DYNAMIC, BITMAP_STATIC } bitmap_type_t;
typedef struct bitmap_data bitmap_t;

struct bitmap_data {
	bitmap_type_t type;
	size_t size;
	size_t count;
	unsigned char *ptr;
	unsigned char buf[0];
};

void bitmap_init(struct bitmap_data *data, size_t bits);
void bitmap_init_static(struct bitmap_data *data, size_t bytes);
size_t bitmap_len(struct bitmap_data *data);
size_t bitmap_sizeof(struct bitmap_data *data);
unsigned char *bitmap_pointer(struct bitmap_data *data);
size_t bitmap_count(struct bitmap_data *data);
size_t bitmap_min(struct bitmap_data *data, int val);
size_t bitmap_max(struct bitmap_data *data, int val);
int bitmap_cmp(struct bitmap_data *data, struct bitmap_data *other);
void bitmap_set(struct bitmap_data *data, size_t pos);
void bitmap_unset(struct bitmap_data *data, size_t pos);
void bitmap_assign(struct bitmap_data *data, size_t pos, int val);
int bitmap_get(struct bitmap_data *data, size_t pos);
int bitmap_bitand(struct bitmap_data *data, size_t pos, struct bitmap_data *with);
void bitmap_clear(struct bitmap_data *data);
void bitmap_resize(struct bitmap_data *data, size_t bits);
void bitmap_free(struct bitmap_data *data);

#endif
