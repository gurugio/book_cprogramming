#include <stdio.h>
#include <stdlib.h>

#include "unittest.h"
#include "cstring.h"


struct priv_data {
	cstring *cstr;
};

static struct priv_data pdata;


static int test_cstring_init(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_cstring_init\n");
	data->cstr = create_cstring("unittest is good", 64);
	return 0;
}

static int test_cstring_final(void *priv)
{
	printf("\ttest_cstring_final\n");
	destroy_cstring(((struct priv_data *)priv)->cstr);
	return 0;
}

static int test_cstring_run(void *priv)
{
	struct priv_data *data = priv;
	cstring *cstr = data->cstr;
	printf("\ttest_cstring_run\n");

	if (cstr->length(cstr) != 16) {
		printf("length failed\n");
	}

	if (cstr->max_size(cstr) != 64) {
		printf("max_size failed\n");
	}

	if (cstr->at(cstr, 3) != 't') {
		printf("at failed\n");
	}

	return 0;
}

#define CHECK_TEST(__condition, __msg) ({ \
			int ____ret_error = !!(__condition);						\
			if (____ret_error) printf("<%s:%d> %s\n",					\
									  __FILE__, __LINE__, __msg);		\
		})

	
static int test_cstring_run_macro(void *priv)
{
	struct priv_data *data = priv;
	cstring *cstr = data->cstr;
	printf("\ttest_cstring_run\n");

	CHECK_TEST(cstr->length(cstr) != 16, "length failed\n");
	CHECK_TEST(cstr->length(cstr) == 16, "length ok\n");
	return 0;
}

static struct unittest test_cstring2 = {
	.init = test_cstring_init,
	.final = test_cstring_final,
	.run = test_cstring_run_macro,
	.priv = &pdata,
};

DEFINE_UNITTEST(cstring2, test_cstring2)
