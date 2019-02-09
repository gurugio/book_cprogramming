#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cstring.h"

void cstring_debug(cstring *this)
{
	if (this->_buf)
		printf("_buf=[%s]\n", this->_buf);
	else
		printf("_buf=[]\n");
	printf("_len=%zu\n", this->_len);
	printf("_max_size=%zu\n", this->_max_size);
}

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

void destroy_cstring(cstring *cstr)
{
	free(cstr->_buf);
	free(cstr);
}
