# 인터페이스과 플러그인을 분리하는 프로그래밍

## 테스트 케이스를 간편하게

이전 test_cstring_run함수는 사실 아무런 테스트도하지 않았지요.
실제로 테스트를 하는 코드를 넣어보겠습니다.

```
static int test_cstring_run(void *priv)
{
	struct priv_data *data = priv;
	cstring *cstr = data->cstr;
	printf("\ttest_cstring_run\n");

	if (cstr->length(cstr) != 16) {
		printf("length failed\n");
	}

	if (cstr->max_size(cstr) != 64) {
		printf("max_size failed\n");
	}

	if (cstr->at(cstr, 3) != 't') {
		printf("at failed\n");
	}

	return 0;
}
```

cstring이 가진 인터페이스들을 테스트합니다.
length인터페이스가 제대로 문자열의 길이를 반환하는지, max_size, at 인터페이스들도 제대로 동작하는지 실행해보고 있습니다.

그런데 만들다보니까 뭔가 단순한 동작을 반복하고있다는게 느껴지지않나요?
눈으로 보기에도 if문이 반복된다는걸 알 수 있습니다.
이전에 자주쓰는 분기문을 매크로를 이용해서 쓰기 쉽게 만드는걸 해봤습니다.
이번에도 똑같이 매크로를 이용해서 반복되는 표현을 쓰기 쉽게 만들어보겠습니다.

```
#define CHECK_TEST(__condition, __msg) ({ \
			int ____ret_error = !!(__condition);						\
			if (____ret_error) printf("<%s:%d> %s\n",					\
									  __FILE__, __LINE__, __msg);		\
		})
	
static int test_cstring_run_macro(void *priv)
{
	struct priv_data *data = priv;
	cstring *cstr = data->cstr;
	printf("\ttest_cstring_run\n");

	CHECK_TEST(cstr->length(cstr) != 16, "length failed\n");
	CHECK_TEST(cstr->length(cstr) == 16, "length ok\n");
	return 0;
}

static struct unittest test_cstring2 = {
	.init = test_cstring_init,
	.final = test_cstring_final,
	.run = test_cstring_run_macro,
	.priv = &pdata,
};
```

CHECK_TEST라는 매크로를 만들었습니다. 몇가지 낯선 문법들이 보일수 있습니다.

``!!(__condition)``는 어떤 표현식이던 그 값을 0혹은 1로 바꾸는 문장입니다. 하나씩 풀어보겠습니다.
* ``(__condition)``: 괄호를 쓰면 어떤 표현의 값을 얻게됩니다. ``((cstr->length(cstr) != 16)``와 같은 표현이라면 참/거짓이 반환되겠지요. ``(cstr->length(cstr)``와 같은 표현이라면 정수가 반환됩니다.
* ``!(__condition)``: ! 표시는 NOT입니다. 표현의 값이 참이면 거짓으로, 0이 아닌 정수면 0으로, 0이면 1로 바꿉니다. 즉 표현식의 값을 0이나 1로 바꾸는 것입니다. 하지만 표현식의 값에 NOT을 했으므로 우리가 원하는 참/거짓값의 반대값을 가집니다.
  * 예를 들어 ``!(cstr->length(cstr))``는 문자열의 길이가 16일때 거짓을 반환합니다.
* ``!!(__condition)``: !가 하나일때는 표현식의 참/거짓값의 반대값을 얻습니다. 다시 NOT을 한번 더 하므로 이제야 제대로된 0/1 값을 얻을 수 있습니다.

그리고 매크로 함수가 
compound statement
http://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs


https://elixir.bootlin.com/linux/latest/source/include/asm-generic/bug.h#L121

만약 gcc가 아닌 다른 컴파일러를 쓰신다면 do-while 문을 이용해서 다음과 같이 만들어도 동일합니다.
```
#define CHECK_TEST(__condition, __msg) do { \
			int ____ret_error = !!(__condition);						\
			if (____ret_error) printf("<%s:%d> %s\n",					\
									  __FILE__, __LINE__, __msg);		\
		} while (0);
```
