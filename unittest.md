# 인터페이스과 플러그인을 분리하는 프로그래밍

## 유닛테스트를 위한 프레임웍 만들기

인터페이스와 플러그인의 활용을 가장 분명하면서도 쉽게 설명해주는 예제중 하나가 유닛테스트 프레임웍입니다.
cstring이라는 라이브러리를 만들었으면 내가 생각한대로 동작하는지 테스트를 해야겠지요.
간단한 main 함수를 만들어서 몇가지 동작을 테스트해볼 수 있을것입니다.

하지만 라이브러리 규모가 커지면서 10개 혹은 100개의 라이브러리를 만들었을때도 각 라이브러리마다 혹은 함수마다 main 함수를 만들 수 있을까요?
만들수는 있겠지만, main함수가 100개있는 프로젝트를 생각해보세요.
소스 구조가 엄청나게 복잡해질 것입니다.

유닛테스트 프레임웍은 각각의 유닛테스트를 등록할 수 있도록 인터페이스를 제공하고, 모아진 유닛테스트들을 한번에 실행해주는 기능을 가집니다.
사용자는 인터페이스에맞게 등록만하고, 프로젝트를 빌드하면 자동으로 유닛테스트 전체를 실행하는 실행파일이 만들어지게됩니다.

아래 그림은 유닛테스트가 대략 어떤 형태를 가지게되는지 보여줍니다.

![unittest interface and dummy plugin](/unittest_dummy.png)

유닛테스트는 2개의 파일로 이루어져있습니다.
1. unittest.c: 프레임웍 구현체, 개별적인 유닛테스트를 등록하는 인터페이스 제공
2. unittest_main.c: 등록된 유닛테스트를 실행

unittest.c에서 2개의 인터페이스를 제공합니다.
1. struct unittest: 유닛테스트의 실행에 필요한 3개의 함수 포인터와 1개의 데이터(void 타입 포인터)가 인터페이스입니다.
1. DEFINE_UNITTEST: 유닛테스트의 이름과 struct unittest를 정의한 객체를 유닛테스트 프레임웍에 저장하는 함수입니다.

### unittest_dummy.c 파일

unittest_dummy.c 파일은 가상의 dummy라는 라이브러리를 가정해서 dummy라는 라이브러리의 유닛테스트를 등록하는 파일입니다.
유닛테스트를 어떻게 사용하는지를 보여주기 위해 만들었습니다.

먼저 unittest.c 파일부터 보면 어떤 일을 하는건지 바로 이해가 안될수도 있습니다.
그러니 먼저 unittest_dummy.c 파일을 보고 유닛테스트 프레임웍을 어떻게 사용해야되는건지를 알아보겠습니다.

```
#include <stdio.h>
#include <stdlib.h>
#include "unittest.h"


struct priv_data {
	int val;
};

static struct priv_data pdata;


int test_dummy_init(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_dummy_init\n");
	data->val = 0xa5a5;
	return 0;
}

int test_dummy_final(void *priv)
{
	struct priv_data *data = priv;
	data->val = 0x0;
	printf("\ttest_dummy_final\n");
	return 0;
}

int test_dummy_run(void *priv)
{
	struct priv_data *data = priv;
	printf("\ttest_dummy_run: val=%x\n", data->val);
	return 0;
}

struct unittest test_dummy = {
	.init = test_dummy_init,
	.final = test_dummy_final,
	.run = test_dummy_run,
	.priv = &pdata,
};

DEFINE_UNITTEST(dummy, test_dummy)
```
헤더중에 unittest.h가 있습니다.
유닛테스트가 제공하는 인터페이스가 정의된 헤더파일입니다.
그림에서 보는것과같이 유닛테스트는 struct unittest라는 구조체와 DEFINE_UNITTEST라는 매크로 함수를 인터페이스로 제공합니다.
unittest_dummy.c 파일은 struct unittest와 DEFINE_UNITTEST라는 인터페이스에 맞게 플러그인을 구현하는 것입니다.
그 플러그인의 목적은 dummy라는 가상의 라이브러리의 테스트 코드가 될것입니다.

먼저 struct unittest 구조체의 객체를 만들기위해 3개의 함수와 1개의 데이터 타입을 구현합니다.

struct priv_data는 테스트 함수들이 공통으로 사용할 데이터들을 모아놓은 것입니다. test_dummy_init, test_dummy_run, test_dummy_final 모든 함수는 이 데이터를 인자로 받습니다. 
임의로 int 타입 변수를 하나 저장해둡니다.

다음은 유닛테스트로 실행될 함수들입니다.
* test_dummy_init: 테스트를 시작하기전에 필요한 준비를 실행합니다. 예를들어 메모리 할당이나 파일 오픈 등이 있습니다. dummy 라이브러리는 하는 일이 없지만 테스트를 위해 공통 데이터에 0xa5a5라는 값을 저장합니다.
* test_dummy_run: 테스트를 실제로 실행하는 함수입니다. test_dummy_init에서 저장한 값이 0xa5a5가 맞는지 검사합니다. 만약 값이 다르다면 에러 메세지를 출력하면 되겠지요.
* test_dummy_final: 테스트가 끝났으니 할당했던 메모리를 해지하거나 열린 파일을 닫는 일 등을 합니다.

이제 unittest_dummy.c가 구현한 함수, 데이터를 struct unittest 타입의 객체에 저장하고, unittest에 등록합니다.
```
struct unittest test_dummy = {
	.init = test_dummy_init,
	.final = test_dummy_final,
	.run = test_dummy_run,
	.priv = &pdata,
};

DEFINE_UNITTEST(dummy, test_dummy)
```

DEFINE_UNITTEST는 dummy라는 이름의 유닛테스트를 등록합니다. dummy라는 유닛테스트가 실행할 함수들은 test_dummy_init, test_dummy_final, test_dummy_run이고, 이 3개 함수가 공통으로 가지는 데이터는 pdata입니다.


## 연습문제

* test_dummy_run은 테스트가 실패하던 성공하던 상관없이 0만 반환합니다. 만약 테스트가 성공하면 0을, 실패하면 -1을 반환하도록 고쳐보세요.
* test_dummy_run의 반환값이 달라졌으니 unittest.c에서 run_test 함수도 반환값을 체크하도록 수정해야합니다. node->test->run을 실행한 후 반환값을 확인해서 테스트가 성공이면 "OK" 메세지를 출력하고, 실패하면 "FAIL" 메세지를 출력하도록 고쳐보세요.


