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
