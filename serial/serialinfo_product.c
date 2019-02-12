#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial_interface.h"


#define PRODUCT_DIGIT 8

int put_product(struct serial_info *info)
{
	char *product;

	info->digit = PRODUCT_DIGIT;

	/* fgets stores the newline and the null characters in the buffer.
	 * So buffer size should be +2.
	 */
	product = calloc(PRODUCT_DIGIT + 2, sizeof(char));
	printf("Input product[%d-digits]:", info->digit);
	fgets(product, PRODUCT_DIGIT + 2, stdin);
	product[strlen(product) - 1] = '\0';

	info->data = product;
	return 0;
}

char *get_product(struct serial_info *info)
{
	return info->data;
}

struct serial_info serialinfo_product= {
	.put = put_product,
	.get = get_product,
};

DEFINE_SERIALINFO(product, serialinfo_product)
