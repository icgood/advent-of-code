#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"
#include "day_base/struct/array.h"

typedef char line_buf[1024];

typedef enum { NONE, LETTER, CONCAT, OR_CONCAT } type_t;

typedef struct {
	type_t type;
	char letter;
	size_t concat_len;
	size_t concat[32];
	size_t or_concat_len;
	size_t or_concat[32];
	char *re;
	regex_t compiled;
} rule_t;

static void parse_rules(line_buf *rule_lines, rule_t *rules, size_t rules_len) {
	for (size_t i=0; i<rules_len; i++) {
		char *ptr = rule_lines[i];
		size_t idx = atoi(ptr);
		rule_t *rule = &rules[idx];
		ptr += strspn(ptr, "0123456789") + 2;

		if (*ptr == '"') {
			rule->type = LETTER;
			rule->letter = *(ptr+1);
		} else {
			while (*ptr != '|' && *ptr != '\n' && *ptr != '\0') {
				rule->concat[rule->concat_len++] = atoi(ptr);
				ptr += strspn(ptr, "0123456789");
				ptr += 1;
			}
			if (*ptr != '|') {
				rule->type = CONCAT;
			} else {
				rule->type = OR_CONCAT;
				ptr += 2;
				while (*ptr != '\n' && *ptr != '\0') {
					rule->or_concat[rule->or_concat_len++] = atoi(ptr);
					ptr += strspn(ptr, "0123456789");
					ptr += 1;
				}
			}
		}
	}
}

static void compile_rules(rule_t *rules) {
	size_t skipped;
	do {
		skipped = 0;
		for (size_t idx=0; idx<256; idx++) {
			rule_t *rule = &rules[idx];
			if (rule->re != NULL || rule->type == NONE) {
				continue;
			} else if (rule->type == LETTER) {
				rule->re = malloc(2);
				snprintf(rule->re, 2, "%c", rule->letter);
			} else if (rule->type == CONCAT) {
				size_t len = 1;
				int should_skip = 0;
				for (size_t i=0; i<rule->concat_len; i++) {
					rule_t *concat_rule = &rules[rule->concat[i]];
					if (concat_rule->re == NULL) {
						should_skip = 1;
						break;
					}
					len += strlen(concat_rule->re);
				}
				if (should_skip) {
					skipped++;
					continue;
				}
				rule->re = malloc(len);
				rule->re[0] = '\0';
				for (size_t i=0; i<rule->concat_len; i++) {
					rule_t *concat_rule = &rules[rule->concat[i]];
					strcat(rule->re, concat_rule->re);
				}
			} else if (rule->type == OR_CONCAT) {
				size_t len = 4;
				int should_skip = 0;
				for (size_t i=0; i<rule->concat_len; i++) {
					rule_t *concat_rule = &rules[rule->concat[i]];
					if (concat_rule->re == NULL) {
						should_skip = 1;
						break;
					}
					len += strlen(concat_rule->re);
				}
				for (size_t i=0; i<rule->or_concat_len; i++) {
					rule_t *or_concat_rule = &rules[rule->or_concat[i]];
					if (or_concat_rule->re == NULL) {
						should_skip = 1;
						break;
					}
					len += strlen(or_concat_rule->re);
				}
				if (should_skip) {
					skipped++;
					continue;
				}
				rule->re = malloc(len);
				rule->re[0] = '\0';
				strcat(rule->re, "(");
				for (size_t i=0; i<rule->concat_len; i++) {
					rule_t *concat_rule = &rules[rule->concat[i]];
					strcat(rule->re, concat_rule->re);
				}
				strcat(rule->re, "|");
				for (size_t i=0; i<rule->or_concat_len; i++) {
					rule_t *or_concat_rule = &rules[rule->or_concat[i]];
					strcat(rule->re, or_concat_rule->re);
				}
				strcat(rule->re, ")");
			}
			size_t rule_len = strlen(rule->re);
			char *final_re = malloc(rule_len + 8);
			snprintf(final_re, rule_len+8, "^%s", rule->re);
			assert(regcomp(&rule->compiled, final_re, REG_EXTENDED) == 0);
			free(final_re);
		}
	} while (skipped > 0);
}

static void free_rules(rule_t *rules) {
	for (size_t i=0; i<256; i++) {
		rule_t *rule = &rules[i];
		if (rule->type != NONE) {
			free(rule->re);
			regfree(&rule->compiled);
			rule->re = NULL;
		}
	}
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	struct array_data array_rule_lines, array_message_lines;
	line_buf *rule_lines;
	line_buf *message_lines;

	array_init(&array_rule_lines, &rule_lines, sizeof (line_buf));
	array_init(&array_message_lines, &message_lines, sizeof (line_buf));

	while (1) {
		line_buf line;
		if (fgets(line, sizeof (line_buf), in) == NULL) break;
		if (strspn(line, "01234567890") == 0) break;
		size_t idx = array_add(&array_rule_lines);
		memcpy(rule_lines[idx], line, sizeof (line_buf));
	}
	while (1) {
		line_buf line;
		if (fgets(line, sizeof (line_buf), in) == NULL) break;
		size_t msg_len = strcspn(line, "\n");
		size_t idx = array_add(&array_message_lines);
		memcpy(message_lines[idx], line, msg_len);
		message_lines[idx][msg_len] = '\0';
	}

	size_t rules_len = array_len(&array_rule_lines);
	size_t messages_len = array_len(&array_message_lines);
	rule_t rules[256];
	memset(rules, 0, 256 * sizeof (rule_t));
	parse_rules(rule_lines, rules, rules_len);
	compile_rules(rules);
	rule_t *first_rule = &rules[0];
	regmatch_t match;

	// part 1
	for (size_t i=0; i<messages_len; i++) {
		if (regexec(&first_rule->compiled, message_lines[i], 1, &match, 0) == 0) {
			if (match.rm_so == 0 && match.rm_eo == strlen(message_lines[i])) {
				res->part1++;
			}
		}
	}

	// part 2
	for (size_t i=0; i<messages_len; i++) {
		char *ptr = message_lines[i];
		size_t num_fortytwo = 0, num_thirtyone = 0;
		while (regexec(&rules[42].compiled, ptr, 1, &match, 0) == 0) {
			num_fortytwo++;
			ptr += match.rm_eo;
		}
		while (regexec(&rules[31].compiled, ptr, 1, &match, 0) == 0) {
			num_thirtyone++;
			ptr += match.rm_eo;
		}
		if (*ptr != '\0') continue;
		if (num_fortytwo > num_thirtyone && num_thirtyone > 0) res->part2++;
	}

	free_rules(rules);
	array_free(&array_rule_lines);
	array_free(&array_message_lines);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {3, 12}},
		{DAY_INPUT, {299, 414}},
	};
	return checks;
}
