# Implement framework and plugin

## Object-oriented programming with structure

We can make a mimic of C++ with structure.
Below is an example to show how to make a class.

```c
#include <stdio.h>
#include <string.h>

struct cstring {
	char *_buf;
	size_t _len;
	size_t _max_size;
	/* http://www.cplusplus.com/reference/string/string/ */
	void (*clear)(struct cstring *this);
	size_t (*length)(struct cstring *this);
	size_t (*max_size)(struct cstring *this);
	char (*at)(struct cstring *this, size_t pos);
	size_t (*compare)(struct cstring *this, struct cstring *cstr);
	void (*print)(struct cstring *this);
	void (*debug)(struct cstring *this);
};
typedef struct cstring cstring;
```
We make a structure and a new type by the name of cstring.
The cstring structure has fields for the buffer to store a string, a length of buffer and a length of stored string, as String class of C++ does.
And also cstring structure has some methods. Please see http://www.cplusplus.com/reference/string/string/ to check what each method does. It is very simple. But please notice that there is one common argument for all methods. The first argument is always a pointer to cstring object. If you knew OOP of the python, you would think it is the same to self argument. Yes, it is the same. Actually C++ provides a syntax sugar to hide the self argument by the name of "this". I do not tell about "this" here because it is out of scope.

Two methods, print and debug, are only for debugging.
Let us see how to implement each method.

```
void cstring_debug(cstring *this)
{
	if (this->_buf)
		printf("_buf=[%s]\n", this->_buf);
	else
		printf("_buf=[]\n");
	printf("_len=%zu\n", this->_len);
	printf("_max_size=%zu\n", this->_max_size);
}
```
The debug method prints the values of member variables in cstring structures. It is for checking the status of object.
The first argument is the pointer to itself. Therefore the name is this as C++ does.
Below is the other methods.
```
void cstring_clear(cstring *this)
{
	for (;this->_len > 0; this->_len--)
		this->_buf[this->_len] = 0;
	this->_buf[this->_len] = 0;
}

size_t cstring_length(cstring *this)
{
	return this->_len;
}

size_t cstring_max_size(cstring *this)
{
	return this->_max_size;
}

char cstring_at(cstring *this, size_t pos)
{
	return (pos < this->_len) ? this->_buf[pos] : 0;
}

void cstring_print(cstring *this)
{
	printf("%s\n", this->_buf);
}
```
They are simple and just example.
The point is that the pointer to object should be passed to the method because each function cannot identify that which object they are involved.

I make a macro to define a cstring class.
```
#define DEFINE_CSTRING(__name, __buf)				\
	struct cstring __name = {						\
		._buf = strdup(__buf),						\
		._len = strlen(__buf),						\
		._max_size = strlen(__buf),					\
		.clear = cstring_clear,						\
		.length = cstring_length,					\
		.max_size = cstring_max_size,				\
		.at = cstring_at,							\
		.print = cstring_print,						\
		.debug = cstring_debug};
```
It is just for reducing hand-work.
Following is an example to show how to make and use a object.

```
int main(void)
{
	char abcd[155] = "abcd";
	DEFINE_CSTRING(aaa, abcd);

	aaa.debug(&aaa);
	aaa.print(&aaa);
	printf("%zu\n", aaa.length(&aaa));
	printf("%zu\n", aaa.max_size(&aaa));
	printf("%c\n", aaa.at(&aaa, 2));

	aaa.clear(&aaa);
	aaa.debug(&aaa);
	printf("%zu\n", aaa.length(&aaa));
	printf("%zu\n", aaa.max_size(&aaa));
	printf("%c\n", aaa.at(&aaa, 2));
	return 0;
}
```
Actually it does not meaningful to make and use a object.
Just storing some variables and function pointers in a structure.

But it would be more similar to object-oriented programming if we made a file and make all function static.
It would be also necessary to make a header.
Please see below sources.

cstring.h
```
#include <stdio.h>
#include <string.h>

struct cstring {
	char *_buf;
	size_t _len;
	size_t _max_size;
	/* http://www.cplusplus.com/reference/string/string/ */
	void (*clear)(struct cstring *this);
	size_t (*length)(struct cstring *this);
	size_t (*max_size)(struct cstring *this);
	char (*at)(struct cstring *this, size_t pos);
	size_t (*compare)(struct cstring *this, struct cstring *cstr);
	void (*print)(struct cstring *this);
	void (*debug)(struct cstring *this);
};
typedef struct cstring cstring;

cstring *create_cstring(char *str, size_t max_size)
```

csting.c
```
#include <stdio.h>
#include <string.h>
#include "cstring.h"

static void cstring_debug(cstring *this)
{
	if (this->_buf)
		printf("_buf=[%s]\n", this->_buf);
	else
		printf("_buf=[]\n");
	printf("_len=%zu\n", this->_len);
	printf("_max_size=%zu\n", this->_max_size);
}

static void cstring_clear(cstring *this)
{
	for (;this->_len > 0; this->_len--)
		this->_buf[this->_len] = 0;
	this->_buf[this->_len] = 0;
}

static size_t cstring_length(cstring *this)
{
	return this->_len;
}

static size_t cstring_max_size(cstring *this)
{
	return this->_max_size;
}

static char cstring_at(cstring *this, size_t pos)
{
	return (pos < this->_len) ? this->_buf[pos] : 0;
}

static void cstring_print(cstring *this)
{
	printf("%s\n", this->_buf);
}


cstring *create_cstring(char *str, size_t max_size)
{
	struct cstring *cstr = calloc(sizeof(*cstr), 1);
	cstr->_buf = strdup(str);
	cstr->_len = strlen(str);
	cstr->_max_size = max_size;
	cstr->clear = cstring_clear;
	cstr->length = cstring_length;
	cstr->max_size = cstring_max_size;
	cstr->at = cstring_at;
	cstr->print = cstring_print;
	cstr->debug = cstring_debug;
	return cstr;
}
```

main.c
```
int main(void)
{
	char abcd[155] = "abcd";
	cstring *aaa = create_cstring(abcd, 155);

	aaa->debug(aaa);
	aaa->print(aaa);
	printf("%zu\n", aaa->length(aaa));
	printf("%zu\n", aaa->max_size(aaa));
	printf("%c\n", aaa->at(aaa, 2));

	aaa->clear(aaa);
	aaa->debug(aaa);
	printf("%zu\n", aaa->length(aaa));
	printf("%zu\n", aaa->max_size(aaa));
	printf("%c\n", aaa->at(aaa, 2));
	return 0;
}
```
Now the methods of cstring is only accessible via cstring object.
Yes, it is more like a namespace.
Actually I do not care whethere it is a namespace or class.
What I want to do is making a seperate space for certain data and functions.
And they are only accessible via a specific interface.
That is usually called as a namespace.

The namespace is a core of the framework and plugin programming.
