# Implement framework and plugin

## A framework for the unittest

I think the best example for the framework and plugin is the unittest framework.
I will describe the design of framework with the unittest implementation.

We made a cstring class and ran some tests in main function.
It could be ok because we have only one library.
Obviously it would not be ok if we have ten or hundreds libraries.
It would be terrible if we need to make so many main functions for each library.
I feel already boring.

Unittest framework provides an interface for user to register a test.
And it runs the all registerred tests for user.

Following picture shows a brief design of unittest framework.

![unittest interface and dummy plugin](/unittest_dummy.png)

Unittest framework consists of two files
* unittest.c: implement framework, provide the interface to register a test
* unittest_main.c: run tests

Two interfaces are provided:
* struct unittest: user need to make an object of struct unittest that consists of three function pointers and one data by void type
* DEFINE_UNITTEST: register the name of user's unittest and the object of struct unittest

### unittest_dummy.c

unittest_dummy.c is an example how to register a dummy unittest.
It registers a virtual unittest for dummy library.
I hope it helps you to understand what the unittest framework is and how it works before looking into the unittest framework implementation.

```
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
	data->val = 0xa5a5;
	return 0;
}

int test_dummy_final(void *priv)
{
	struct priv_data *data = priv;
	data->val = 0x0;
	printf("\ttest_dummy_final\n");
	return 0;
}

int test_dummy_run(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_dummy_run: val=%x\n", data->val);
	return 0;
}

struct unittest test_dummy = {
	.init = test_dummy_init,
	.final = test_dummy_final,
	.run = test_dummy_run,
	.priv = &pdata,
};

DEFINE_UNITTEST(dummy, test_dummy)
```

The header file unittest.h defines the interfaces for user: struct unittest and DEFINE_UNITTEST.
unittest_dummy.c should implements struct unittest and DEFINE_UNITTEST: that is what plugin does.

We needs three functions and one data for struct unittest.
* test_dummy_init: initialize or prepare test. For example, it could open a file or allocate memory. The example code just set a value to check all functions work well.
* test_dummy_run: run some tests here. The example code just check the value is correct.
* test_final: cleanup function. It could close a file or free memory.
* test_dummy: This object passes three functions and one data to the framework.

struct priv_data is a common data for all three functions: test_dummy_init, test_dummy_run and test_dummy_final.
test_dummy is defined as following and passed to DEFINE_UNITTEST.

```
struct unittest test_dummy = {
	.init = test_dummy_init,
	.final = test_dummy_final,
	.run = test_dummy_run,
	.priv = &pdata,
};

DEFINE_UNITTEST(dummy, test_dummy)
```

DEFINE_UNITTEST macro registeres a test_dummy object by the name of dummy.
The unittest framework will call the three functions with pdata argument.

### unittest.h

Can you catch how to use the unittest framework?
Now let us see the unittest framework source.

The unittest.h file defines the interfaces.

```
#include <stdio.h>

struct unittest {
	int (*init)(void *);
	int (*final)(void *);
	int (*run)(void *);
	void *priv;
};

#define DEFINE_UNITTEST(__name, __struct_unittest)	\
	void register_##__name(void) {					\
		register_test(#__name, &__struct_unittest);	\
	}

#define REGISTER_UNITTEST(__name)    \
	void register_##__name(void);	 \
	register_##__name()

int register_test(char *name, struct unittest *test);
int run_test(void);
```

struct unittest includes three function pointers and one pointer to void type data.
The data type is void pointer so that we can register any data.

DEFINE_UNITTEST macro function has two parameters:
* ``__name``: the name of user's unittest
* ``__struct_unittest``: the name of object of struct unittest

DEFINE_UNITTEST macro generates a function calling register_test.
DEFINE_UNITTEST in unittest_dummy.c generates register_dummy function that calls register_test.
What is it for?
Why does not user call register_test directly?

For example, what happens if unittest_dummy.c makes my_dummy function and calls register_test as following?

```
void my_dummy(void)
{
	register_test("dummy", &test_dummy);
}
```
Then where is the my_dummy function called?
The main function can call my_dummy.

```
void main(void)
{
	my_dummy();
}
```
And other unittests registers their own functions.
Each register function has different name.
So main would be like following.

```
void main(void)
{
	my_dummy();
	call_register_XXX();
	register_YYY();
	...
}
```
If the unittest framework defines a common interface DEFINE_UNITTEST, all unittests have register_"name" function.
And main function uses REGISTER_UNITTEST macro function to call all register_"name" functions.

```
void main(void)
{
	REGISTER_UNITTEST(dummy);
	REGISTER_UNITTEST(cstring);
	REGISTER_UNITTEST(mem_alloc);
	...
}
```

Which main function looks better?
Which main function looks better for maintenance?

### unittest.c

unittest.c should implement register_test funuction that receives a name and an object of struct unittest.
And it should make a set including all names and objects.

unittest.c should also implement run_test function that runs all tests in the set.
Following is an example of unittest.c.

```
#include <stdio.h>
#include <stdlib.h>
#include "unittest.h"

struct test_node {
	char *name;
	struct unittest *test;
	struct test_node *next;
};

struct test_node *head;


int register_test(char *name, struct unittest *test)
{
	struct test_node *new_node = calloc(1, sizeof(*new_node));

	new_node->name = name;
	new_node->test = test;
	new_node->next = NULL;

	if (head == NULL) {
		head = new_node;
	} else {
		struct test_node *old = head;
		head = new_node;
		new_node->next = old;
	}

	printf("test[%s] is registered\n", name);
	return 0;
}

int run_test(void)
{
	struct test_node *node;

	for (node = head; node != NULL; node = node->next) {
		printf("run [%s]\n", node->name);
		node->test->init(node->test->priv);
		node->test->run(node->test->priv);
		node->test->final(node->test->priv);
	}
	return 0;
}
```
It defines a list of struct test_node.
register_test function creates an object of test_node and store data from each unittest.
And it adds the test_node object into the list.
run_test function traversals the list and run test one by one.

### unittest_main.c 파일

Following is an example of unittest_main.c.

```c
#include <stdio.h>
#include "unittest.h"


int main(void)
{
	REGISTER_UNITTEST(dummy);
	run_test();
	return 0;
}
```

dummy unittest is registered by REGISTER_UNITTEST macro.
And all tests are executed by run_test function.

When you want to add an unittest, you do:
1. make a file unittest_"name".c
2. make an object of struct unittest with three functions and one data
3. call DEFINE_UNITTEST
4. add REGISTER_UNITTEST(name) in main

It does not matter how many unittest files are there, or how many developers who want to register their tests are there.
There are one common interface and all unittest use the common interface.
Each unittest is plugin of the unittest framework.

## 연습문제
* test_dummy_run returns only 0. Fix it to return 0, if test succeeds, -1, if test fails.
* Fix the message of run_test. If test succeeds, it prints "OK", or "FAIL".
* register_test creates a list and add nodes. There is no function to clean up the memory of the list. Create a function to clean up. Where is that function called?
