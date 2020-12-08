#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passport.h"

static char *read_value(value *v, char *line) {
	v->len = strcspn(line, " \n");
	strncpy(v->data, line, sizeof (line_buf));
	return line + v->len;
}

static void print_value(char *name, value *v) {
	printf("%s: %.*s\n", name, v->len, v->data);
}

static int read_fields(passport *p, char *line) {
	line += strspn(line, " \n");
	if (strcspn(line, ":") != 3) return 0;

	if (strncmp(line, "byr", 3) == 0) {
		line = read_value(&p->byr, line+4);
	} else if (strncmp(line, "iyr", 3) == 0) {
		line = read_value(&p->iyr, line+4);
	} else if (strncmp(line, "eyr", 3) == 0) {
		line = read_value(&p->eyr, line+4);
	} else if (strncmp(line, "hgt", 3) == 0) {
		line = read_value(&p->hgt, line+4);
	} else if (strncmp(line, "hcl", 3) == 0) {
		line = read_value(&p->hcl, line+4);
	} else if (strncmp(line, "ecl", 3) == 0) {
		line = read_value(&p->ecl, line+4);
	} else if (strncmp(line, "pid", 3) == 0) {
		line = read_value(&p->pid, line+4);
	} else if (strncmp(line, "cid", 3) == 0) {
		p->has_cid = 1;
		line = read_value(&p->cid, line+4);
	}

	return 1 + read_fields(p, line);
}

void passport_print(passport *p) {
	print_value("byr", &p->byr);
	print_value("iyr", &p->iyr);
	print_value("eyr", &p->eyr);
	print_value("hgt", &p->hgt);
	print_value("hcl", &p->hcl);
	print_value("ecl", &p->ecl);
	print_value("pid", &p->pid);
	print_value("cid", &p->cid);
	printf("\n");
}

int passport_read(passport *p, FILE *f) {
	memset(p, 0, sizeof (passport));

	size_t num_lines = 0;
	do {
		char *current_line = p->lines[num_lines++];
		if (fgets(current_line, sizeof (line_buf), f) == NULL) break;

		int line_fields = read_fields(p, current_line);
		if (line_fields == 0) break;
		p->num_fields += line_fields;
	} while (num_lines < 9);

	return p->num_fields;
}

int passport_has_fields(passport *p) {
	return p->num_fields == 8 || (p->num_fields == 7 && p->has_cid == 0);
}

static int valid_byr(value *v) {
	if (v->len != 4) return 0;
	return strncmp(v->data, "1920", 4) >= 0 && strncmp(v->data, "2002", 4) <= 0;
}

static int valid_iyr(value *v) {
	if (v->len != 4) return 0;
	return strncmp(v->data, "2010", 4) >= 0 && strncmp(v->data, "2020", 4) <= 0;
}

static int valid_eyr(value *v) {
	if (v->len != 4) return 0;
	return strncmp(v->data, "2020", 4) >= 0 && strncmp(v->data, "2030", 4) <= 0;
}

static int valid_hgt(value *v) {
	if (v->len == 5) {
		if (strncmp(v->data+3, "cm", 2) != 0) return 0;
		return strncmp(v->data, "150", 3) >= 0 && strncmp(v->data, "193", 3) <= 0;
	} else if (v->len == 4) {
		if (strncmp(v->data+2, "in", 2) != 0) return 0;
		return strncmp(v->data, "59", 2) >= 0 && strncmp(v->data, "76", 2) <= 0;
	} else {
		return 0;
	}
}

static int valid_hcl(value *v) {
	if (v->len != 7 || v->data[0] != '#') return 0;
	return strspn(v->data+1, "abcdef0123456789") == 6;
}

static int valid_ecl(value *v) {
	return v->len == 3 && (strncmp(v->data, "amb", 3) == 0
		|| strncmp(v->data, "blu", 3) == 0
		|| strncmp(v->data, "brn", 3) == 0
		|| strncmp(v->data, "gry", 3) == 0
		|| strncmp(v->data, "grn", 3) == 0
		|| strncmp(v->data, "hzl", 3) == 0
		|| strncmp(v->data, "oth", 3) == 0);
}

static int valid_pid(value *v) {
	return v->len == 9 && strspn(v->data, "0123456789") == 9;
}

int passport_is_valid(passport *p) {
	if (!passport_has_fields(p)) {
		return 0;
	}

	return valid_byr(&p->byr)
		&& valid_iyr(&p->iyr)
		&& valid_eyr(&p->eyr)
		&& valid_hgt(&p->hgt)
		&& valid_hcl(&p->hcl)
		&& valid_ecl(&p->ecl)
		&& valid_pid(&p->pid);
}
