#include <stdio.h>
#include "unittest.h"


int main(void)
{
	REGISTER_UNITTEST(dummy);
	run_test();
	return 0;
}
