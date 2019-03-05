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

```c
  	for (i = 0; i < sizeof(chandlers)/sizeof(chandlers[0]); i++) {
		  if (chandlers[i].cmd == cmd) {
			  ret = chandlers[i].handler(chandlers[i].cmd);
			  break;
		  }
	  }
```
for 루프에서 배열의 각 요소를 순회하게되고, 그 중에서 사용자가 입력한 명령과 동일한 명령어가있는 요소을 찾은 다음, 해당 요소에 저장된 함수 포인터를 호출합니다.

좋은 프로그램을 만들 때는 항상 이 요구사항이 바뀔 수 있다는걸 생각해야합니다. 우리가 만든 cmd_handler 구조체의 배열이 어떻게 바뀔 수 있을까요? 프로그램의 규모가 커지거나, 사용자가 입력할 명령어가 바뀌면, 구조체 배열은 어떻게 바껴야될까요?

프로그램의 규모가 매우 커져서, 여러 파일에서 정의된 여러 함수들이 chandlers 배열을 참조한다면 어떤 문제가 생길까요? 매번 똑같은 for 루프 함수를 여러 파일에서 여러번 똑같이 써주어야합니다. 만약 구조체 이름이 바뀌거나, 구조체의 정의 자체가 바뀐다면, 여러 파일의 for 루프를 모두 수정해야합니다. 프로그램 스펙을 하나만 바꾸려고해도 많은 코드를 바꿔야하는 문제가 생깁니다.

사용자가 입력할 명령어가 동적으로 바뀌면 어떤 문제가 생길까요? 현재는 사용자 명령어가 미리 정의되어있으므로, 소스 코드에 하드코딩되어 정의되어있습니다. 그래서 배열을 사용해도 아무런 문제가 없었지요. 하지만 사용자의 명령을 사용자가 직접 지정하거나 추가/삭제할 수 있도록 만든다면, 배열이 적당한 자료구조일까요? 배열은 가장 빠르고 만들기쉬운 데이터 구조이지만, 데이터를 추가/삭제하기 어려다는 단점도 가지고 있습니다. 그럴때는 링크드 리스트를 사용하도록 코드를 수정하면 좋습니다. 다시 정리하면 프로그램 요구사항이 바뀌어서 사용자의 명령을 동적으로 추가하고 삭제해야할 경우, cmd_handler 데이터 구조체를 링크드 리스트에 맞게 고쳐주고, chandler 배열도 링크드 리스트로 바꿔야됩니다. 또한 프로그램 전체를 뒤져서 for 루프도 리스트를 순회하는 코드로 바꿔야겠네요.

결국 프로그램의 요구사항이 조금만 바뀌어도 for 루프는 수정될수밖에 없습니다. 만약 cmd_handler 구조체가 한두번만 사용되는 데이터라면 상관없겠지만, 여러 파일에서 여러번 사용된 데이터라면 for 루프 코드는 매우 유연하지못한 코드입니다.

그럼 어떻게해야 유연한 코드가 될까요? 우리는 if-else를 단순화할 때 매크로를 이용해서 동일한 형태로 반복되는 함수들을 정의하는 코드를 봤습니다. 같은 형태의 함수가 반복될때, 그 함수를 생성하는 매크로를 만들어서 매크로를 여러번 호출하기만하면 함수를 만들어낼 수 있었습니다.

for 루프 또한 매크로를 이용해서 정의할 수 있습니다.
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
