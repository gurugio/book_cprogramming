# Make for-loop with macro

In previous chapter, we created a (command, handler function) structure and an array of the structure for all user commands.

```c
struct cmd_handler {
	char cmd;
	int (*handler)(char);
};

struct cmd_handler chandlers[] = {
	{'a', macro_handler_a},
	{'b', macro_handler_b},
	{'c', macro_handler_c},
	{'d', macro_handler_d},
	{'e', macro_handler_e}};
```
Below for-loop scans the array and find out the handler function of the given command.
Then it calls the handler function.

```c
  	for (i = 0; i < sizeof(chandlers)/sizeof(chandlers[0]); i++) {
		  if (chandlers[i].cmd == cmd) {
			  ret = chandlers[i].handler(chandlers[i].cmd);
			  break;
		  }
	  }
```

Please note that the specification is always changed.
We made an array of cmd_handler structure.
What happens if the program should add and remove the user command dynamically?
Array is not suitable for add and removing some elements.
So we should make a list of cmd_handler structure.
That is not all.
We also should change the for-loop to traversal the list.

Is that enough?
Sometimes not.
If the program lives long and get bigger, many files would access the chandlers.
Therefore we need to change many files whenever the cmd_handler structure is changed and/or chandler is changed.
It is not general at all.

If the cmd_handler structure is defined and used only once, we do not need to care.
But if it is common data for many files or many layers, it should be more general to access the cmd_handler structure.

What are we supporsed to do to make a general for-loop?
In previous chapter, we have seen that the macro can make a common interface to replace a repeated function.
The for-loop is also repeated.
So we can make a macro to replace the for-loop.

```c
#define cmd_for_each(__i, __cmd, __fp, __handlers) \
	for(__i = 0, __cmd=__handlers[0].cmd, __fp=__handlers[0].handler; \
	    __i < sizeof(__handlers)/sizeof(__handlers[0]); \
	    __i++, __cmd=__handlers[__i].cmd, __fp=__handlers[__i].handler)
```
cmd_for_each macro function generates a for-loop to traversal the array of cmd_handler objects.
Whenever you search one command in cmd_handler array, you can use cmd_for_each macro function instead of making for-loop.

What is its benefit? What is it for?
For a very simple example, what happens if we need to change the name of cmd field into command?
Or if we need to change the type of cmd field into int type?
We should find out all for-loop which access the chandler and change the code, and then build all files again.
But if we used cmd_for_each macro, we would fix only cmd_for_each macro.

```c
#define cmd_for_each(__i, __cmd, __fp, __handlers) \
	for(__i = 0, __cmd=__handlers[0].command, __fp=__handlers[0].handler; \
	    __i < sizeof(__handlers)/sizeof(__handlers[0]); \
	    __i++, __cmd=__handlers[__i].command, __fp=__handlers[__i].handler)
```

Let us change long-if.c file to use cmd_for_each macro.

```c
#include <stdio.h>
#include <unistd.h>


struct cmd_handler {
	char cmd;
	int (*handler)(char);
};

#define cmd_for_each(__i, __cmd, __fp, __handlers) \
	for(__i = 0, __cmd=__handlers[0].cmd, __fp=__handlers[0].handler; \
	    __i < sizeof(__handlers)/sizeof(__handlers[0]); \
	    __i++, __cmd=__handlers[__i].cmd, __fp=__handlers[__i].handler)


/* use gcc option -E to debug macro functions: gcc -e long-if.c */
#define DEFINE_HANDLER(__cmd) static int macro_handler_##__cmd(char arg_cmd) { \
		printf("command is %s\n", #__cmd);			\
		return arg_cmd - 'a' + 1;				\
	}

DEFINE_HANDLER(a)
DEFINE_HANDLER(b)
DEFINE_HANDLER(c)
DEFINE_HANDLER(d)
DEFINE_HANDLER(e)

struct cmd_handler chandlers[] = {
	{'a', macro_handler_a},
	{'b', macro_handler_b},
	{'c', macro_handler_c},
	{'d', macro_handler_d},
	{'e', macro_handler_e}};

int macro_if_foreach(char cmd)
{
	int i;
	int ret = -1;
	char each_cmd;
	int (*each_fp)(char);
	
	cmd_for_each(i, each_cmd, each_fp, chandlers) {
		if (each_cmd == cmd) {
			ret = each_fp(each_cmd);
			break;
		}
	}

	if (ret < 0)
		printf("Unidentified command\n");
	return ret;
}

int main(int argc, char *argv[])
{
	char cmd;

	if (argc != 2) {
		printf("usage: ./a.out command(a|b|c|d|e)\n");
		return 1;
	}

	printf("result=%d\n", macro_if_foreach((char)argv[1][0]));
	return 0;
}

```

## Excercises
* Change the name of chandlers into cmd_handlers. What are we supporsed to change to call cmd_for_each?
* The main function prints "usage: ./a.out command(a|b|c|d|e)" message if user does not input any argument. It is hard coded. Hard coding is always problematic. Make a help() function and change the main function as below. help() function should use cmd_for_each macro and print the user commands supported by our program.
```c
void help(struct cmd_handler *handlers)
{
	/* show commands that program supports: "usage: ./a.out command(a|b|c|d|e)" */
}
int main(int argc, char *argv[])
{
	char cmd;

	if (argc != 2) {
		help(&chandlers)
		return 1;
	}

	printf("result=%d\n", macro_if_foreach((char)argv[1][0]));
	return 0;
}
```
* Move help() and macro_if_foreach() functions to another file. Where should we move the macro definitions?
