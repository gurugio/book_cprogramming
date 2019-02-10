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

### unittest.h, unittest.c 파일

그럼 이제 유닛테스트 프레임웍의 코드를 보겠습니다.

다음은 unittest.h 파일입니다. 유닛테스트가 어떤 인터페이스를 제공하는지를 알아볼 수 있습니다.
```
#include <stdio.h>

struct unittest {
	int (*init)(void *);
	int (*final)(void *);
	int (*run)(void *);
	void *priv;
};

#define DEFINE_UNITTEST(__name, __struct_unittest)	\
	void register_##__name(void) {					\
		register_test(#__name, &__struct_unittest);	\
	}

#define REGISTER_UNITTEST(__name)    \
	void register_##__name(void);	 \
	register_##__name()

int register_test(char *name, struct unittest *test);
int run_test(void);
```

가장 먼저 보이는게 unittest라는 구조체입니다. 위에 unittest_dummy.c에서 봤듯이 각각의 유닛테스트를 등록할 때 사용되는 구조체입니다.
* init: 테스트를 시작하기전에 준비할 것들을 실행
* final: 테스트가 끝난 후 마무리
* run: 테스트 실행

DEFINE_UNITTEST 매크로 함수도 보입니다. 2개의 인자를 가집니다.
* ``__name``: 유닛테스트의 이름
* ``__struct_unittest``: 유닛테스트가 등록하고자하는 struct unittest 객체의 이름

DEFINE_UNITTEST 매크로는 register_test를 호출하는 함수를 만들어줍니다.
unittest_dummy.c의 경우에는 register_dummy라는 함수를 만듭니다.
왜 DEFINE_UNITTEST 매크로가 필요할까요? 사용자가 register_test를 바로 호출하면 안될까요?

예를 들어 unittest_dummy.c에서 다음과 같이 register_test를 호출했다고 가정해봅시다.
```
void my_dummy(void)
{
	register_test("dummy", &test_dummy);
}
```
my_dummy함수에서 register_test를 호출했습니다.
그럼 my_dummy함수는 어디에서 호출될까요?
다음과같이 main함수에서 호출됩니다.

```
void main(void)
{
	my_dummy();
}
```

그 외에 다양한 유닛트세트 케이스들이 register_test를 나름대로 호출하겠지요.
main함수는 다음과 같이 될 것입니다.

```
void main(void)
{
	my_dummy();
	call_register_dummy();
	register_dummy();
	...
}
```
하지만 DEFINE_UNITTEST 매크로를 제공해서 모든 유닛테스트 케이스들이 공통으로 register_"유닛테스트이름" 함수를 생성하도록 만들면 모든 유닛테스트 등록함수는 공통된 이름을 같게 됩니다.
그래서 결국 REGISTER_UNITEST 매크로를 사용할 수 있게되고, main함수는 다음과 같은 모양이됩니다.

```
void main(void)
{
	REGISTER_UNITTEST(dummy);
	REGISTER_UNITTEST(cstring);
	REGISTER_UNITTEST(mem_alloc);
	...
}
```
인터페이스는 여러 사용자 혹은 여러 모듈이 따를수있는 공통된 규칙을 주는게 목적입니다.
DEFINE_UNITTEST, REGISTER_UNITTEST 매크로 함수는 유닛테스트를 등록할 수 있는 범용적인 인터페이스를 제공하는 좋은 방법이 됩니다.

unittest.c 파일의 코드를 보기 전에 한번 생각해보세요.
unittest.c 파일에 어떤 코드가 들어가야할지 어느정도 구상이되지않나요?
register_test함수는 등록된 유닛테스트 이름과 struct unittest 객체를 묶어서 하나의 집합을 만들면 됩니다.
그리고 run_test함수는 유닛테스트 집합에서 유닛테스트를 하나씩 꺼내면서 각 함수 포인터를 실행해주면 되겠지요.
집합을 만드는 방법은 여러가지입니다. 유닛테스트의 갯수가 많지않다면 배열, 리스트를 이용할 수도 있고, 만약 수천개 이상의 유닛테스트가 등록되는 대규모 프로젝트라면 트리나 힙을 만들어야겠지요.

제가 만든 unittest.c 파일은 간단하게 리스트를 이용했습니다.
```
#include <stdio.h>
#include <stdlib.h>
#include "unittest.h"

struct test_node {
	char *name;
	struct unittest *test;
	struct test_node *next;
};

struct test_node *head;


int register_test(char *name, struct unittest *test)
{
	struct test_node *new_node = calloc(1, sizeof(*new_node));

	new_node->name = name;
	new_node->test = test;
	new_node->next = NULL;

	if (head == NULL) {
		head = new_node;
	} else {
		struct test_node *old = head;
		head = new_node;
		new_node->next = old;
	}

	printf("test[%s] is registered\n", name);
	return 0;
}

int run_test(void)
{
	struct test_node *node;

	for (node = head; node != NULL; node = node->next) {
		printf("run [%s]\n", node->name);
		node->test->init(node->test->priv);
		node->test->run(node->test->priv);
		node->test->final(node->test->priv);
	}
	return 0;
}
```

유닛테스트의 리스트를 만들기위해 struct test_node라는 구조체를 만들었습니다.
register_test함수는 test_node 구조체의 리스트를 만드는 일을 하는게 전부입니다.
run_test함수는 리스트를 순회하는게 전부이지요.

### unittest_main.c 파일

이제 유닛테스트를 실행하는 main함수를 보겠습니다.
```
#include <stdio.h>
#include "unittest.h"


int main(void)
{
	REGISTER_UNITTEST(dummy);
	run_test();
	return 0;
}
```

REGISTER_UNITTEST를 이용해서 dummy의 유닛테스트를 등록하고, run_test를 호출하는 것으로 등록된 유닛테스트를 실행해줍니다.

앞으로 새로운 유닛테스트를 추가해주려면 이렇게하면 됩니다.
1. unittest_"유닛테스트이름".c를 만들고, struct unittest 객체를 만든 후 DEFINE_UNITTEST 호출
2. main함수에 REGISTER_UNITTEST(유닛테스트이름) 호출

유닛테스트가 아무리 많아져도 하나의 main함수로 관리할 수 있으니 프로젝트 구조도 단순해지고 관리도 편해집니다.
잘만든 인터페이스가 프로젝트 전체의 구조를 단순화시킬 수 있습니다.
크던 작던 좋은 소프트웨어를 만들기 위해서 단순히 추상적 디자인만 잘하면되는게 아니라 프로그래밍에도 능숙한 사람이 좋은 코드 디자인을 해야한다는 것을 기억하세요.

## 연습문제

* test_dummy_run은 테스트가 실패하던 성공하던 상관없이 0만 반환합니다. 만약 테스트가 성공하면 0을, 실패하면 -1을 반환하도록 고쳐보세요.
* test_dummy_run의 반환값이 달라졌으니 unittest.c에서 run_test 함수도 반환값을 체크하도록 수정해야합니다. node->test->run을 실행한 후 반환값을 확인해서 테스트가 성공이면 "OK" 메세지를 출력하고, 실패하면 "FAIL" 메세지를 출력하도록 고쳐보세요.
* unittest.c에서 struct test_node의 리스트를 생성해서 각 유닛테스트를 관리합니다. 하나의 유닛테스트가 등록될때마다 하나의 리스트 노드가 추가됩니다. 그런데 프로그램이 끝날때 리스트를 해지하는 코드가 없습니다. run_test 함수 마지막에 리스트 노드들의 메모리를 해지하는 코드를 추가해보세요.
* 시간이 있다면 유닛테스트 집합 관리를 리스트가 아닌 힙이나 트리로 만들어보세요. 좋은 데이터구조 복습이 될것입니다.
