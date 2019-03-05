# Make for-loop general

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
No.
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
cmd_for_each 매크로 함수는 cmd_handler 구조체의 배열을 순회하는 for 루프를 만들어냅니다. 그리고 cmd_handler 구조체를 이용해야할 때마다 for 루프를 코딩하는게 아니라 cmd_for_each 매크로를 호출하기만하면 됩니다.

아주 간단한 예를 하나 생각해보겠습니다. 만약 cmd_handler 구조체에서 사용자 명령을 저장하는 cmd 필드의 이름을 command로 바꿔야한다면 어떻게하면 될까요? 만약 소스 파일 여기저기에 있는 for 루프를 일일이 찾아서 고쳐야한다면 매우 귀찬을 것입니다. cmd_for_each 매크로를 이용한다면 다음과 같이 cmd_for_each 매크로만 수정하면 됩니다.

```c
#define cmd_for_each(__i, __cmd, __fp, __handlers) \
	for(__i = 0, __cmd=__handlers[0].command, __fp=__handlers[0].handler; \
	    __i < sizeof(__handlers)/sizeof(__handlers[0]); \
	    __i++, __cmd=__handlers[__i].command, __fp=__handlers[__i].handler)
```

그럼 long-if.c 예제 코드를 cmd_for_each 매크로를 쓰도록 바꿔보겠습니다.

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

## 연습문제
* chandlers라는 배열 이름을 cmd_handlers로 바꿔보세요. cmd_for_each 매크로를 호출할때 무엇을 어떻게 바꿔야할까요?
* 현재 main함수는 사용자가 아무런 명령도 입력하지않았을때 "usage: ./a.out command(a|b|c|d|e)"라는 메세지를 출력합니다. 이렇게 하드코딩된 메세지는 당연히 좋지않겠지요. help()라는 함수를 하나 만들어서 main함수를 다음과 같이 바꿔보세요. help함수는 chandlers 배열을 순회하면서 프로그램이 지원하는 사용자 명령들을 출력해야합니다. cmd_for_each 매크로를 이용하세요.
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
* help 함수와 macro_if_foreach 함수를 다른 파일로 옮겨보세요. 매크로 정의들을 
