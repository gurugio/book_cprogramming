# Sample project: serial number generator

## A framework for serial number

We cannot guess what kind of data we will use to generate a serial number.

![serial2](/serial2.png)

But there is two common attributes in all input data.
* All data is a string of number or alphabet.
* All data has a limited digit.

Actually we do not need to know what kind of data it is.
We only need to know the string and the maximum digit of the data.
And the two attributes are the interfaces for the plugin.
One attribute becomes one plugin.

![serial3](/serial3.png)

We can make some interfaces as following.

```
#include <stdio.h>

struct serial_info {
	char *data;
	int digit;
	int (*put)(struct serial_info *info);
	char *(*get)(struct serial_info *info);
};

#define DEFINE_SERIALINFO(__name, __struct_info)	\
	void register_##__name(void) {					\
		register_info(#__name, &__struct_info);	\
	}

#define REGISTER_SERIALINFO(__name)    \
	void register_##__name(void);	 \
	register_##__name()

int register_info(char *name, struct serial_info *info);
int create_serial(void);
```
struct serial_info includes string and digit.
put function requests data from user.
get function passes user data to framework.
DEFINE_SERIALINFO and REGISTER_SERIALINFO are what we saw in unittest framework.
DEFINE_SERIALINFO is called by plugin and REGISTER_SERIALINFO is called by main.

Following is a source of serial number framwork: serial_interface.c

```
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
```

Yes, it is almost same to the unittest framework.
It receives data from plugin and stores it in the list.
When it generates serial number it calls function of each plugin.
put function of plugin gets data from user and get function passes the user data to framework.

Following is a plugin for the user ID: serialinfo_id.c

```
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
```

``put_id`` function is what we already saw in ``serial_simple.c``.
It requests 4-digit data to user and stores the data into the object serial_info serialinfo_id.
It also stores the digit of data, 4, into the object. 
And finally DEFINE_SERIALINFO is called to register the user ID to the framework.

```
#include <stdio.h>
#include <string.h>
#include "serial_interface.h"

int main(void)
{
	/* new info should be added at top */
	REGISTER_SERIALINFO(id);
	
	create_serial();
    
	return 0;
}
```
REGISTER_SERIALINFO is called to register "id" plugin.
create_serial function just calls put and get interfaces of each plugin.
Everything is the same to the unittest framework.

## Exercise
* ``serialinfo_id.c`` stores user ID into ``serial_info serialinfo_id`` object. Make another plugin to stores the product number into ``serial_info serialinfo_product`` object. File name would be ``serialinfo_product.c``. Do not forget to register the plugin in main function.

## more requirement
User should input the user ID and other data manually. It is not good. Change the program to read a file and generate the many serial numbers. The file has format as below and program should generate three serial numbers.
PRODUCT NUMBER
USER-ID1
USER-ID2
USER-ID3

## more requirement
We have a serial number. How can we check if the serial number is valid?
Make a verification program that read a serial number and print user ID and product number.
The input data is common for the generator and verificator.
We cannot change both whenever the input data is changed.
How can you make the generator and verificator share the input data?

## more requirement
A real program usually have a configuration file, serial.conf, as below that defines a name and digit of input data.
```
user-id 4
product-id 8
```
Make a serial number generator read serial.conf and generate a serial number.
You need to generate plugin for each data dynamically.

## more requirement
Make another plugin that defines encryption algorithm.
You might need openssl library.

