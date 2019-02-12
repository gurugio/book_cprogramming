#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "serial_interface.h"

struct serial_node {
	char *name;
	struct serial_info *info;
	struct serial_node *next;
};

struct serial_node *head;


int register_info(char *name, struct serial_info *info)
{
	struct serial_node *new_node = calloc(1, sizeof(*new_node));

	new_node->name = name;
	new_node->info = info;
	new_node->next = NULL;

	if (head == NULL) {
		head = new_node;
	} else {
		struct serial_node *old = head;
		head = new_node;
		new_node->next = old;
	}

	printf("serial-info [%s] is registered\n", name);
	return 0;
}

int create_serial(void)
{
	struct serial_node *node;
	char *serial_buffer = calloc(64, sizeof(char));
	int digit = 0;

	for (node = head; node != NULL; node = node->next) {
		node->info->put(node->info);
		digit += node->info->digit;
		strcat(serial_buffer, node->info->get(node->info));
	}

	printf("Serial[%d]: [%s]\n", digit, serial_buffer);
	return 0;
}
