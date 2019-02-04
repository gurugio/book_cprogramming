#include <stdio.h>
#include "unittest.h"


int main(void)
{
	REGISTER_UNITTEST(dummy);
	REGISTER_UNITTEST(cstring);
	run_test();
	return 0;
}
