# 인터페이스과 플러그인을 분리하는 프로그래밍

## cstring의 유닛테스트

위에서 말했듯이 새로운 유닛테스트를 추가해주려면 이렇게하면 됩니다.
1. unittest_"유닛테스트이름".c를 만들고, struct unittest 객체를 만든 후 DEFINE_UNITTEST 호출
2. main함수에 REGISTER_UNITTEST(유닛테스트이름) 호출

cstring의 유닛테스트를 만드려면 2가지 단계가 필요합니다.
1. unittest_cstring.c를 만들어서 struct unittest객체를 만들고 DEFINE_UNITTEST 호출
2. main함수에 REGISTER_UNITTEST(cstring) 호출

### unittest_cstring.c

```
#include <stdio.h>
#include <stdlib.h>

#include "unittest.h"
#include "cstring.h"


struct priv_data {
	cstring *cstr;
};

static struct priv_data pdata;


int test_cstring_init(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_cstring_init\n");
	data->cstr = create_cstring("unittest is good", 64);
	return 0;
}

int test_cstring_final(void *priv)
{
	printf("\ttest_cstring_final\n");
	destroy_cstring(((struct priv_data *)priv)->cstr);
	return 0;
}

int test_cstring_run(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_cstring_run\n");
	data->cstr->debug(data->cstr);
	return 0;
}

struct unittest test_cstring = {
	.init = test_cstring_init,
	.final = test_cstring_final,
	.run = test_cstring_run,
	.priv = &pdata,
};

DEFINE_UNITTEST(cstring, test_cstring)
```
