#include <stdio.h>


struct cstring {
	char *buf;
	size_t len;
	size_t max;
};

#define DEFINE_CSTRING(__name, __buf, __len, __max) \
	struct cstring __name = { \
		.buf = __buf, \
		.len = __len, \
		.max = __max};

/* api: cstrcpy, cstrcmp, cstrcat, cstrlen, cstrchr */



int main(void)
{
	char abcd[155];
	DEFINE_CSTRING(aaa, abcd,0,155);

	cstrcpy(aaa.buf, "asdf");
	printf("%s\n", aaa.buf);
	
}
