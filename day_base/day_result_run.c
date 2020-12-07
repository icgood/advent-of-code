#include <stdio.h>
#include "day_base/day_result.h"

int main() {
	day_result res = {};
	day_result_compute(&res, stdin);
	day_result_print(&res);
	return 0;
}
