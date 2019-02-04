#include <stdio.h>
#include <stdlib.h>
#include "unittest.h"


struct priv_data {
	int val;
};

static struct priv_data pdata;


int test_dummy_init(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_dummy_init\n");
	return 0;
}

int test_dummy_final(void *priv)
{
	printf("\ttest_dummy_final\n");
	return 0;
}

int test_dummy_run(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_dummy_run\n");
	return 0;
}

struct unittest test_dummy = {
	.init = test_dummy_init,
	.final = test_dummy_final,
	.run = test_dummy_run,
	.priv = &pdata,
};

DEFINE_UNITTEST(dummy)
{
	register_test("dummy", &test_dummy);
}
