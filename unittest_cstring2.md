# Implement framework and plugin

## Make test case with macro

In previous example, test_cstring_run function does not test anything.
Let us add some test as following.

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
It checks if return values of length, max_size and at methods.

There is a pettern for all if-statements.
It only compares two values and prints a message.
We have made a macro to replace a repeated if-else-statement and for-loop before.
What about making a macro to replace the if-statement as following?

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

Now we use printf to print a message but it would be necessary to print a message on log file.
If we make a macro to check values and print a message, it will be more flexible to print error messages.
And of couse, the test function will be shorter.

Let us see the CHECK_TEST macro.
``!!(__condition)`` statement might be strange for someone.
It translates any statement into the logical values: 0 or 1.
Let us look into it step by step.
* ``(__condition)``: 괄호를 쓰면 어떤 표현의 값을 얻게됩니다. ``((cstr->length(cstr) != 16)``와 같은 표현이라면 참/거짓이 반환되겠지요. ``(cstr->length(cstr)``와 같은 표현이라면 정수가 반환됩니다.
* ``!(__condition)``: ! 표시는 NOT입니다. 표현의 값이 참이면 거짓으로, 0이 아닌 정수면 0으로, 0이면 1로 바꿉니다. 즉 표현식의 값을 0이나 1로 바꾸는 것입니다. 하지만 표현식의 값에 NOT을 했으므로 우리가 원하는 참/거짓값의 반대값을 가집니다.
  * 예를 들어 ``!(cstr->length(cstr))``는 문자열의 길이가 16일때 거짓을 반환합니다.
* ``!!(__condition)``: !가 하나일때는 표현식의 참/거짓값의 반대값을 얻습니다. 다시 NOT을 한번 더 하므로 이제야 제대로된 0/1 값을 얻을 수 있습니다.


* ``(__condition)``: 괄호를 쓰면 어떤 표현의 값을 얻게됩니다. ``((cstr->length(cstr) != 16)``와 같은 표현이라면 참/거짓이 반환되겠지요. ``(cstr->length(cstr)``와 같은 표현이라면 정수가 반환됩니다.
* ``!(__condition)``: ! 표시는 NOT입니다. 표현의 값이 참이면 거짓으로, 0이 아닌 정수면 0으로, 0이면 1로 바꿉니다. 즉 표현식의 값을 0이나 1로 바꾸는 것입니다. 하지만 표현식의 값에 NOT을 했으므로 우리가 원하는 참/거짓값의 반대값을 가집니다.
  * 예를 들어 ``!(cstr->length(cstr))``는 문자열의 길이가 16일때 거짓을 반환합니다.
* ``!!(__condition)``: !가 하나일때는 표현식의 참/거짓값의 반대값을 얻습니다. 다시 NOT을 한번 더 하므로 이제야 제대로된 0/1 값을 얻을 수 있습니다.

그리고 매크로 함수가 지역변수를 가지고있고, ``({...})``로 둘러싸여있습니다. 이것은 중첩문이라고 불리는 것입니다. GCC에서만 지원하는 문법입니다.
* compound statement: http://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs

쉽게 생각하면 여러 문장을 하나로 묶을 때 ``({..})``로 묶는다고만 생각하면 됩니다. 이렇게 묶은 표현들이 특정 값을 반환할 수도 있습니다.

매크로의 활용에 대해 좀더 알고싶으신 분들은 커널 소스를 읽어보시면 도움이 되실 것입니다.
* https://elixir.bootlin.com/linux/latest/source/include/asm-generic/bug.h#L121

만약 gcc가 아닌 다른 컴파일러를 쓰신다면 do-while 문을 이용해서 다음과 같이 만들어도 동일합니다.
```
#define CHECK_TEST(__condition, __msg) do { \
			int ____ret_error = !!(__condition);						\
			if (____ret_error) printf("<%s:%d> %s\n",					\
									  __FILE__, __LINE__, __msg);		\
		} while (0);
```
