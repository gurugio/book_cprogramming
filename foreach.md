# 자주쓰는 for 루프를 간편하게

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
  
  	for (i = 0; i < sizeof(chandlers)/sizeof(chandlers[0]); i++) {
		  if (chandlers[i].cmd == cmd) {
			  ret = chandlers[i].handler(chandlers[i].cmd);
			  break;
		  }
	  }
```



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
