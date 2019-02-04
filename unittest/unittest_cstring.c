#include <stdio.h>
#include <stdlib.h>

int test_cstring_init(void *pri)
{
	return 0;
}

int test_cstring_final(void *pri)
{
	return 0;
}

int test_cstring_run(void *pri)
{
	return 0;
}

struct unittest test_cstring = {
	.init = test_cstring_init,
	.final = test_cstring_final,
	.run = test_cstring_run,
};

int register_cstring(void)
{
	register_test("cstring", &test_cstring);
	return 0;
}
