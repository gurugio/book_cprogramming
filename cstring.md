# 프레임웍과 플러그인을 분리하는 프로그래밍

## 구조체를 이용한 객체지향 프로그래밍

C언어를 이용해서 객체지향 프로그래밍를 흉내낼 수 있습니다. 예제 소스를 보면서 어떻게 가능한지를 한번 알아보겠습니다.

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
우리는 C++의 String 클래스를 C로 만들어보겠습니다. 위와같이 string이라는 이름의 구조체를 만듭니다. String 클래스의 속성은 문자열을 저장할 버퍼와 버퍼의 길이, 실제로 저장된 문자열의 길이가 있습니다.

그리고 클래스에 들어갈 메소드들이 있습니다. 각각의 메소드들이 하는 일은 http://www.cplusplus.com/reference/string/string/ 사이트를 참고하세요. 각 메소드들에는 공통점이 있습니다. 반드시 첫번째 인자가 struct cstring 구조체의 포인터이어야합니다. 왜인지는 실제 예제 코드를 보면서 설명하겠습니다.

print와 debug 메소드는 C++의 String 클래스에 없는 메소드입니다. 제가 디버깅을 위해서 임의로 만들것들입니다.

다음으로 각 메소드를 어떻게 구현하는지 보겠습니다.
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
먼저 debug메소드입니다. cstring 구조체에있는 변수들을 출력합니다. 클래스의 멤버 변수들을 출력해주는 것과 같습니다. 객체가 어떤 상태인지 알아보기위한 함수입니다.

함수 인자는 cstring 객체 자신입니다. 그래서 this라는 이름을 썼습니다. C++에서도 객체 자신의 주소를 this라는 이름으로 표현하는 것과 같습니다.

다른 메소드들의 구현 코드를 보겠습니다.
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
나머지 메소드들의 구현은 위와 같습니다. 아주 단순하게 구현했습니다. 중요한건 객체 자신을 인자로 넘겨야한다는 것입니다. 함수 내부에서는 이 함수가 어떤 객체의 메소드인지 알 수가 없으니까 반드시 함수 인자에 객체의 포인터를 넘겨줘야합니다.

아래는 객체를 생성하는 매크로입니다.
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
매크로를 사용하지않았다면 객체를 선언할 때마다 반복해서 하드코딩했을 선언부분을 매크로로 만든것 뿐입니다.

아래는 객체를 사용하는 main함수입니다. 어떻게 객체를 만들고 사용하는지를 보여주기 위해 만들었습니다.
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
사실 객체를 만드는 것 자체에 의미는 없습니다. 단지 구조체에 함수 포인터와 속성 등을 저장해서 하나의 클래스처럼 사용할 수도 있다는 것을 보여주는 것 뿐입니다.

cstring_length 함수는 전역 함수이니까 굳이 cstring 객체를 통해서 호출할 필요가 없습니다. 하지만 아래와 같이 하나의 파일을 만들어서 각 함수들을 static으로 선언해주고, struct cstring만 헤더 파일에서 정의하면 어떻게 될까요?

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

이렇게 cstring 관련 메소드들을 외부에서 호출하지못하도록 구현하면 cstring이 클래스가 아니라 namespace의 역할을 하게됩니다. cstring 객체를 만드는게 중요한게 아니라 어떤 데이터와 함수들을 하나의 namespace에 모아놓고, 반드시 정해진 인터페이스(cstring객체의 메소드)를 통해서만 해당 데이터와 함수들을 이용하도록 만들게됩니다. 

C언어로 클래스를 만들 수 있다는 것보다 더 중요한게 바로 namespace를 만드는 것입니다. 그리고 이런 namespace를 활용해서 프레임웍과 플러그인 프로그래밍을 할 수 있습니다.

## 연습문제

* 코드를 잘보면 불합리한게 보입니다. ``aaa.length(&aaa)``, ``aaa.at(&aaa,2)`` 와 같이 객체의 메소드들을 호출할 때마다 객체 포인터를 넘겨야합니다. 반복되는 코드이니 당연히 매크로를 이용해서 쉽게 사용할 수 있도록 바꿀 수 있겠지요. 매크로의 이름을 CALL_CSTRING_LENGTH, CALL_CSTRING_AT 등으로 만들어보세요.
