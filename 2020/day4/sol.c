#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passport.h"
#include "day_base/day_result.h"

static passport *new_passports(size_t new_size, passport *old_passports, size_t len) {
	passport *new_passports = malloc(new_size * sizeof(passport));
	if (len > 0) {
		memcpy(new_passports, old_passports, len * sizeof(passport));
	}
	free(old_passports);
	return new_passports;
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	size_t passports_len = 0, passports_size = 2;
	passport *passports = new_passports(passports_size, NULL, 0);

	while (1) {
		passport *current = &passports[passports_len];
		if (passport_read(current, in) == 0) {
			break;
		}
		passports_len++;
		if (passports_len >= passports_size) {
			passports = new_passports(passports_size *= 2, passports, passports_len);
		}
	}

	// part 1
	for (size_t i=0; i<passports_len; i++) {
		if (passport_has_fields(&passports[i])) {
			res->part1++;
		}
	}
	
	// part 2
	for (size_t i=0; i<passports_len; i++) {
		if (passport_is_valid(&passports[i])) {
			res->part2++;
		}
	}

	free(passports);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {2, 2}},
		{DAY_INPUT, {192, 101}},
	};
	return checks;
}
