# 자주쓰는 for 루프를 간편하게

길어지는 if-else 코드를 줄이기위해 다음과 같이 (명령어, 명령어 수행함수) 구조체를 만들고, 구조체의 배열로 모든 명령어와 명령어 수행 함수를 지정할 수 있었습니다.

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
이렇게 만들어진 데이터를 처리하는 코드는 다음과 같은 for 루프 코드입니다.

```c
  	for (i = 0; i < sizeof(chandlers)/sizeof(chandlers[0]); i++) {
		  if (chandlers[i].cmd == cmd) {
			  ret = chandlers[i].handler(chandlers[i].cmd);
			  break;
		  }
	  }
```
for 루프는 배열의 각 요소를 순회하게되고, 그 중에서 사용자가 입력한 명령과 동일한 명령어가있는 요소을 찾은 다음, 해당 요소에 저장된 함수 포인터를 호출합니다.

좋은 프로그램을 만들 때는 항상 이 요구사항이 바뀔 수 있다는걸 생각해야합니다. 우리가 만든 cmd_handler 구조체의 배열이 어떻게 바뀔 수 있을까요? 프로그램의 규모가 커지거나, 사용자가 입력할 명령어가 바뀌면, 구조체 배열은 어떻게 바껴야될까요?

프로그램의 규모가 매우 커져서, 여러 파일에서 정의된 여러 함수들이 chandlers 배열을 참조한다면 어떤 문제가 생길까요? 매번 똑같은 for 루프 함수를 여러 파일에서 여러번 똑같이 써주어야합니다. 만약 구조체 이름이 바뀌거나, 구조체의 정의 자체가 바뀐다면, 여러 파일의 for 루프를 모두 수정해야합니다. 프로그램 스펙을 하나만 바꾸려고해도 많은 코드를 바꿔야하는 문제가 생깁니다.

사용자가 입력할 명령어가 동적으로 바뀌면 어떤 문제가 생길까요? 현재는 사용자 명령어가 미리 정의되어있으므로, 소스 코드에 하드코딩되어 정의되어있습니다. 그래서 배열을 사용해도 아무런 문제가 없었지요. 하지만 사용자의 명령을 사용자가 직접 지정하거나 추가/삭제할 수 있도록 만든다면, 배열이 적당한 자료구조일까요?



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
