#include <stdio.h>
#include <stdlib.h>

typedef char line_buf[1024];

typedef struct {
	char *data;
	int len;
} value;

typedef struct {
	line_buf lines[9];
	int has_cid;
	int num_fields;

	value byr;
	value iyr;
	value eyr;
	value hgt;
	value hcl;
	value ecl;
	value pid;
	value cid;
} passport;

void passport_print(passport *p);
int passport_read(passport *p, FILE *f);
int passport_has_fields(passport *p);
int passport_is_valid(passport *p);
