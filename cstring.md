# 구조체를 이용한 객체지향 프로그래밍




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

## 연습문제

* DEFINE_CSTRING 매크로는 정적으로 cstring 구조체의 객체를 생성합니다. 당연히 cstring 객체를 동적으로 생성해야할 경우도 있겠지요. cstring 객체를 동적으로 생성해주는 함수를 만들어보세요. 함수의 타입은 ``cstring *create_cstring(char *str, size_t max_size)``입니다.
