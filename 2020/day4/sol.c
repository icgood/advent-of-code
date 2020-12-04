#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passport.h"

static passport *new_passports(size_t new_size, passport *old_passports, size_t len) {
	passport *new_passports = malloc(new_size * sizeof(passport));
	if (len > 0) {
		memcpy(new_passports, old_passports, len * sizeof(passport));
	}
	free(old_passports);
	return new_passports;
}

int main() {
	size_t passports_len = 0, passports_size = 2;
	passport *passports = new_passports(passports_size, NULL, 0);

	while (1) {
		passport *current = &passports[passports_len];
		if (passport_read(current, stdin) == 0) {
			break;
		}
		passports_len++;
		if (passports_len >= passports_size) {
			passports = new_passports(passports_size *= 2, passports, passports_len);
		}
	}
	//for (size_t i=0; i<passports_len; i++) {
	//	passport_print(&passports[i]);
	//}

	// part 1
	int part1 = 0;
	for (size_t i=0; i<passports_len; i++) {
		if (passport_has_fields(&passports[i])) {
			part1++;
		}
	}
	printf("part 1: %d\n", part1);
	
	// part 2
	int part2 = 0;
	for (size_t i=0; i<passports_len; i++) {
		if (passport_is_valid(&passports[i])) {
			part2++;
		}
	}
	printf("part 2: %d\n", part2);

	free(passports);
	return 0;
}
