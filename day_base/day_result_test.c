#include <assert.h>
#include "day_base/day_result.h"

int main(int argc, char **argv) {
	assert(argc >= 2);
	day_result_test(argv[1]);
	return 0;
}
