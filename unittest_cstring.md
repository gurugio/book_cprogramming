# Implement framework and plugin

## cstring unittest

Let me remind you how to add new unittest.
1. create unittest_"name".c and define an object of struct unittest, then call DEFINE_UNITTEST.
2. add REGISTER_UNITTEST(name) in main function


위에서 말했듯이 새로운 유닛테스트를 추가해주려면 이렇게하면 됩니다.
1. unittest_"유닛테스트이름".c를 만들고, struct unittest 객체를 만든 후 DEFINE_UNITTEST 호출
2. main함수에 REGISTER_UNITTEST(유닛테스트이름) 호출

cstring의 유닛테스트를 만드려면 2가지 단계가 필요합니다.
1. unittest_cstring.c를 만들어서 struct unittest객체를 만들고 DEFINE_UNITTEST 호출
2. main함수에 REGISTER_UNITTEST(cstring) 호출

### unittest_cstring.c

struct unittest의 객체를 만들려면 3개의 함수와 1개의 데이터를 만들어야합니다. 다시한번 struct unittest를 생각해보면 다음과 같습니다.
* init함수
* final함수
* run함수
* 각 함수들이 사용할 데이터 struct priv_data

init, final, run 함수들이 사용할 데이터가 무엇일까요?
경우에 따라 다르겠습니다만, 대부분 바로 테스트하려는 데이터구조가 될 것입니다.
unittest_cstring.c에서 struct priv_data에는 cstring이 들어갑니다.

그러면 init 함수에서 할 일도 정해집니다. 바로 cstring 객체를 만드는 것입니다. 그래야 run함수에서 cstring 객체를 테스트할 수 있으니까요. final 함수에서 할 일도 정해지네요. cstring 객체를 해지하는 것입니다.

물론 run함수에서 cstring 객체를 생성하고 해지할 수 있습니다. 그렇게한다면 init과 final에서는 할일이 없겠네요. 그래도 괸찬습니다. 유닛테스트를 만드는 사람이 결정할 사항입니다.

unittest_cstring.c의 코드는 다음과 같습니다.

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

아주 짧은 코드이므로 일일이 설명하지않겠습니다. 각 함수가 무엇을해야하는지를 생각하면서 과연 해야할 일을 하는지를 잘 살펴보시기 바랍니다.

파일의 마지막은 DEFINE_UNITTEST를 호출해서 유닛테스트를 등록하는 것으로 끝납니다. 만약 추가로 다른 유닛테스트를 만든다고해도 대부분 DEFINE_UNITTEST 호출로 끝나겠지요.


## 연습문제
* 사실 test_cstring_run함수에서 테스트를 하지 않습니다. 단지 cstring이 가지는 인터페이스 하나를 호출할 뿐입니다. 실제로 테스트를 하려면 뭘 테스트해야하고 어떻게 해야할까요? 다음장에서 실제 테스트 코드를 추가해보겠지만 미리 생각해보세요.
