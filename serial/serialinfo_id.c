#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial_interface.h"


#define ID_DIGIT 4

int put_id(struct serial_info *info)
{
	char *id;

	info->digit = ID_DIGIT;

	/* fgets stores the newline and the null characters in the buffer.
	 * So buffer size should be +2.
	 */
	id = calloc(ID_DIGIT + 2, sizeof(char));
	printf("Input User ID[%d-digits]:", info->digit);
	fgets(id, ID_DIGIT + 2, stdin);
	id[strlen(id) - 1] = '\0';

	info->data = id;
	return 0;
}

char *get_id(struct serial_info *info)
{
	return info->data;
}

struct serial_info serialinfo_id= {
	.put = put_id,
	.get = get_id,
};

DEFINE_SERIALINFO(id, serialinfo_id)
