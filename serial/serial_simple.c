#include <stdio.h>
#include <string.h>

int make_serial(char *serial, int serial_len,
                char *id, int id_len,
                char *product, int product_len)
{
	int i;
    
	if (id_len + product_len != serial_len) {
		printf("Wrong input: id=4-digit product=8-digit\n");
		return 0;
	}
	else
	{
		strncat(serial, id, id_len);
		strncpy(serial + id_len, product, product_len);
	}

	return strlen(serial);
}

int main(void)
{
	char id[32];
	char product[32];
	char serial[13] = {0,};
	int ret;
    
	printf("Input User ID[4-digits]:");
	fgets(id, 32, stdin);
	id[strlen(id) - 1] = '\0';

	printf("Input Product Number[8-digit]:");
	fgets(product, 32, stdin);
	product[strlen(product) - 1] = '\0';

	ret = make_serial(serial, 12,
			  id, strlen(id),
			  product, strlen(product));
	printf("Serial[%d]=[%s]\n", ret, serial);
    
	return 0;
}
