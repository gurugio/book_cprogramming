# Sample project: serial number generator

## Initial version

Unittest framework shows you the design concept of the framework and plugin.
I would like to show you one more example which are a little bit more practical.

It is a serial number generator.
It is very common to input a serial number when we install a software such like Windows.
Have you ever imagine how Microsoft generates the serial number?

Let us assume that the serial generator gets two data and returns 12-digit alphabet and number.

![serial1](/serial1.png)

We do not consider encryption to keep it simple.
Let us try a simplest example that gets user ID and product number.
And it returns 12-digit serial that is just a combination of the user ID and product number.
It should be work as following.
* request the user ID
* request the product number
* allocate 12-byte buffer
* copy the user ID
* copy the product number after the user ID
* print the serial number

```
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
```

It is so simple.
But as I said several times, program requirements are always changed.
Let us imagine some scenarios about requirement changes:
* There are some clients who request a trial version for one month. We should add a expire date into the serial number.
* Some clients who bought another product want to try this product. We do not need to add the expire date but we need to disable some features.
* So many people buy this product. 4-digit user ID is expired. We should extend the user ID to 8-digit.

What do you think about them? Are they possible scenarios?

The problem is those requirements are not thrown to us when we design the program.
Marketing team and managers always throw more and more requirements if the product is sold more and more.
Yes, of course, when we upgrade the product, we also have some requirements.
We does not feel only happy but also panic.

Finally make_serial function would be like following because more and more information should be inserted into the serial number.

```
int make_serial(char *serial, int serial_len,
		char *id, int id_len,
		char *product, int product_len,
		char *expire, int expire_len,
		int trial_type,
		int crypt_type)
{
......
}
```
Do you know how many parameters a function can support?
I do not know exactly.
But what I know is that it is not good.

## Excercise
* try to implement a function ``int make_serial(char *serial, int serial_len, char *id, int id_len, char *product, int product_len, char *expire, int expire_len, int trial_type, int crypt_type)``. Just copy all information into the serial number. Can you see a pettern? Is it similar pattern that we saw in previous chapter?
