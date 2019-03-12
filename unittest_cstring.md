# Implement framework and plugin

## cstring unittest

Let me remind you how to add new unittest.
1. create unittest_"name".c and define an object of struct unittest, then call DEFINE_UNITTEST.
2. add REGISTER_UNITTEST(name) in main function

We will do it for cstring.
1. create unittest_cstring.c
2. define an object of struct unittest and call DEFINE_UNITTEST in unittest_cstring.c
3. add REGISTER_UNITTEST(cstring) in main

### unittest_cstring.c

We need to make three functions and one data: init, final, run and struct priv_data.

First what can be the common data for the three functions?
It might be what we want to test.
unittest_cstring.c stores cstring object in the data.

Then what init function does is decided: it should create an object of cstring class.
And run function will do some tests and final function will free the object.

Following is unittest_cstring.c file.

```
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
```

It is not necessary to desribe it again.
It is very short and I already describe what each function does.

Please do not forget to finish the file with DEFINE_UNITTEST.


## Exercise
* Actually test_cstring_run does not test anything. Add some test code there.
