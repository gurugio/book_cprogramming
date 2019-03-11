# Implement framework and plugin

## A framework for the unittest

I think the best example for the framework and plugin is the unittest framework.
I will describe the design of framework with the unittest implementation.

We made a cstring class and ran some tests in main function.
It could be ok because we have only one library.
Obviously it would not be ok if we have ten or hundreds libraries.
It would be terrible if we need to make so many main functions for each library.
I feel already boring.

Unittest framework provides an interface for user to register a test.
And it runs the all registerred tests for user.

Following picture shows a brief design of unittest framework.

![unittest interface and dummy plugin](/unittest_dummy.png)

Unittest framework consists of two files
* unittest.c: implement framework, provide the interface to register a test
* unittest_main.c: run tests

Two interfaces are provided:
* struct unittest: user need to make an object of struct unittest that consists of three function pointers and one data by void type
* DEFINE_UNITTEST: register the name of user's unittest and the object of struct unittest

### unittest_dummy.c

unittest_dummy.c is an example how to register a dummy unittest.
It registers a virtual unittest for dummy library.
I hope it helps you to understand what the unittest framework is and how it works before looking into the unittest framework implementation.

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

The header file unittest.h defines the interfaces for user: struct unittest and DEFINE_UNITTEST.
unittest_dummy.c should implements struct unittest and DEFINE_UNITTEST: that is what plugin does.

We needs three functions and one data for struct unittest.
* test_dummy_init: initialize or prepare test. For example, it could open a file or allocate memory. The example code just set a value to check all functions work well.
* test_dummy_run: run some tests here. The example code just check the value is correct.
* test_final: cleanup function. It could close a file or free memory.
* test_dummy: This object passes three functions and one data to the framework.

struct priv_data is a common data for all three functions: test_dummy_init, test_dummy_run and test_dummy_final.
test_dummy is defined as following and passed to DEFINE_UNITTEST.

```
struct unittest test_dummy = {
	.init = test_dummy_init,
	.final = test_dummy_final,
	.run = test_dummy_run,
	.priv = &pdata,
};

DEFINE_UNITTEST(dummy, test_dummy)
```

DEFINE_UNITTEST macro registeres a test_dummy object by the name of dummy.
The unittest framework will call the three functions with pdata argument.

### unittest.h, unittest.c

Can you catch how to use the unittest framework?
Now let us see the unittest framework source.

The unittest.h file defines the interfaces.

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

struct unittest includes
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
집합을 만드는 방법은 여러가지입니다. 유닛테스트의 갯수가 많지않다면 배열, 리스트를 이용할 수도 있고, 만약 수천개 이상의 유닛테스트가 등록되는 대규모 프로젝트라면 다른 자료구조를 써야할 수도 있습니다. 유닛테스트의 목록을 별도의 파일에 텍스트로 저장하고 유닛테스트의 main함수에서 파일을 읽어서 유닛테스트를 실행하도록 만들수도있겠지요. 방식이야 

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
* 시간이 있다면 유닛테스트 집합 관리를 리스트가 아닌 힙이나 트리로 만들어보세요. 힙이나 트리로 만들면 리스트에 비해서 무엇이 좋아질까요? 힌트를 주자면 리스트는 노드의 추가와 삭제가 리스트의 끝이나 시작 부분에서만 일어나야합니다. 중간에있는 노드를 삭제하거나 추가하려면 효율이 떨어집니다. 
