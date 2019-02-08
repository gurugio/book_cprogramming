#include <stdio.h>
#include <stdlib.h>

#include "unittest.h"
#include "cstring.h"


struct priv_data {
	cstring *cstr;
};

static struct priv_data pdata;


int test_cstring_init(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_cstring_init\n");
	data->cstr = create_cstring("unittest is good", 64);
	return 0;
}

int test_cstring_final(void *priv)
{
	printf("\ttest_cstring_final\n");
	destroy_cstring(((struct priv_data *)priv)->cstr);
	return 0;
}

int test_cstring_run(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_cstring_run\n");
	data->cstr->debug(data->cstr);
	return 0;
}

struct unittest test_cstring = {
	.init = test_cstring_init,
	.final = test_cstring_final,
	.run = test_cstring_run,
	.priv = &pdata,
};

DEFINE_UNITTEST(cstring, test_cstring)
