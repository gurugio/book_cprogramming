#include <stdio.h>
#include "unittest.h"


int main(void)
{
	REGISTER_UNITTEST(dummy);
	REGISTER_UNITTEST(cstring);
    REGISTER_UNITTEST(cstring2);
	run_test();
	return 0;
}
