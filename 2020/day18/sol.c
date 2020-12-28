#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "day_base/day_result.h"

typedef unsigned long int num_t;
typedef enum { ADD, MUL } op_t;
typedef enum { NUM, NESTED } type_t;

struct expression_t {
	op_t op;
	type_t type;
	num_t num;
	struct expression_t *nested;
	size_t nested_len;
};
typedef struct expression_t expression_t;

static size_t parse(char *line, size_t i, expression_t *parent) {
	i += strspn(line+i, " ");
	op_t op = ADD;

	if (line[i] == ')' || line[i] == '\n' || line[i] == '\0') {
		return i + 1;
	} else if (line[i] == '+') {
		op = ADD;
		i++;
		i += strspn(line+i, " ");
	} else if (line[i] == '*') {
		op = MUL;
		i++;
		i += strspn(line+i, " ");
	}

	expression_t *exp = &parent->nested[parent->nested_len++];
	exp->op = op;
	if (line[i] == '(') {
		exp->type = NESTED;
		exp->nested = calloc(64, sizeof (expression_t));
		exp->nested_len = 0;
		i = parse(line, i+1, exp);
	} else {
		exp->type = NUM;
		exp->num = atoi(line+i);
		i += strspn(line+i, "0123456789");
	}
	i = parse(line, i, parent);
	return i;
}

static void parse_lines(char **lines, size_t num_lines, expression_t *root) {
	*root = (expression_t) {ADD, NESTED};
	root->nested = calloc(512, sizeof (expression_t));
	root->nested_len = 0;
	for (size_t i=0; i<num_lines; i++) {
		char *line = lines[i];
		expression_t *exp = &root->nested[root->nested_len++];
		*exp = (expression_t) {ADD, NESTED};
		exp->nested = calloc(64, sizeof (expression_t));
		exp->nested_len = 0;
		parse(line, 0, exp);
	}
}

static void evaluate_expression(expression_t *exp) {
	if (exp->type == NESTED) {
		for (size_t i=0; i<exp->nested_len; i++) {
			evaluate_expression(&exp->nested[i]);
		}
		exp->num = 0;
		for (size_t i=0; i<exp->nested_len; i++) {
			if (exp->nested[i].op == ADD) exp->num += exp->nested[i].num;
			if (exp->nested[i].op == MUL) exp->num *= exp->nested[i].num;
		}
	}
}

static void evaluate_expression_p2(expression_t *exp) {
	if (exp->type == NESTED) {
		for (size_t i=0; i<exp->nested_len; i++) {
			evaluate_expression_p2(&exp->nested[i]);
		}
		exp->num = 1;
		size_t num_operands = 1;
		num_t operands[exp->nested_len];
		for (size_t i=0; i<exp->nested_len; i++) operands[i] = 0;
		for (size_t i=0; i<exp->nested_len; i++) {
			if (exp->nested[i].op == MUL) num_operands++;
			operands[num_operands-1] += exp->nested[i].num;
		}
		for (size_t i=0; i<num_operands; i++) {
			exp->num *= operands[i];
		}
	}
}

static void free_expression(expression_t *exp) {
	if (exp->type == NESTED) {
		for (size_t i=0; i<exp->nested_len; i++) {
			free_expression(&exp->nested[i]);
		}
		free(exp->nested);
	}
	memset(exp, 0, sizeof (expression_t));
}

void day_result_compute(char *arg, day_result_t *res, FILE *in) {
	char *lines[512];
	size_t num_lines;
	expression_t root;

	for (num_lines=0; num_lines<512; num_lines++) {
		lines[num_lines] = malloc(1024);
		if (fgets(lines[num_lines], 1024, in) == NULL) break;
	}

	// part 1
	parse_lines(lines, num_lines, &root);
	evaluate_expression(&root);
	res->part1 = root.num;
	free_expression(&root);

	// part 2
	parse_lines(lines, num_lines, &root);
	evaluate_expression_p2(&root);
	res->part2 = root.num;
	free_expression(&root);

	for (size_t i=0; i<=num_lines; i++) free(lines[i]);
}

day_check_t *day_check_provide() {
	static day_check_t checks[] = {
		{"example", {122, 282}},
		{DAY_INPUT, {86311597203806, 276894767062189}},
	};
	return checks;
}
