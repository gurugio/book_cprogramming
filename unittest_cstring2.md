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
* ``(__condition)``: return the value of a statement. ``(cstr->length(cstr)`` returns a integer value.
* ``!(__condition)``: ! means NOT. What is "NOT" of a integer value? If the value is 0, it is 1. If the value is not 0, it is 0. Therefore !() translates a arithmatic value into a logical value.
  * ``!(cstr->length(cstr))`` will be 0, if the length is 16.
* ``!!(__condition)``: What is NOT of NOT? It is nothing. So this is a final logical value. If the integer value of the condition is not zero, this is 1 (TRUE), and vice versa.

Please notice that the macro function has a local variable.
That is what the macro is wrapped with ``({..})``.
It called as the compound statement of GCC compiler.
* compound statement: http://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html#Statement-Exprs

If you would like to know more about the compound statement please read kernel code:
* https://elixir.bootlin.com/linux/latest/source/include/asm-generic/bug.h#L121

If you use another compiler, you can use do-while as following.
```
#define CHECK_TEST(__condition, __msg) do { \
			int ____ret_error = !!(__condition);						\
			if (____ret_error) printf("<%s:%d> %s\n",					\
									  __FILE__, __LINE__, __msg);		\
		} while (0);
```
