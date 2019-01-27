길어지는 조건문

어떤 프로그램이 사용자로부터 (a, b, c, d, e)중에 하나의 명령을 입력받고, 입력받은 명령이 무엇인지를 출력한다고 생각해보겠습니다. 대략 이런 코드가 될 것입니다.

if cmd == ‘a’
	do_something_for_a();
else if cmd == ‘b’
	do_something_for_b();
......(생략)
else
	do_error();

하나의 if 문이 하나의 명령어를 처리합니다. 총 5개의 명령어를 처리해야하니까 5개의 if 문을 만들고, 마지막 else는 에러처리가 되겠네요.

실제로 동작하는 코드를 만들어보겠습니다.


#include <stdio.h>
#include <unistd.h>

int long_if(char cmd)
{
	int ret;

	switch (cmd) {
	case 'a':
		printf("command is a\n");
		/* code for command a */
		ret = 1;
		break;
	case 'b':
		printf("command is b\n");
		/* code for command a */
		ret = 2;
		break;
	case 'c':
		printf("command is c\n");
		/* code for command a */
		ret = 3;
		break;
	case 'd':
		printf("command is d\n");
		/* code for command a */
		ret = 4;
		break;
	case 'e':
		printf("command is e\n");
		/* code for command a */
		ret = 5;
		break;
	default:
		printf("Unidentified command\n");
		/* code for error */
		ret = -1;
		break;
	}
	return ret;
}


int main(int argc, char *argv[])
{
	char cmd;

	if (argc != 2) {
		printf("usage: ./a.out command(a|b|c|d|e)\n");
		return 1;
	}

	printf("result=%d\n", long_if((char)argv[1][0]));
	return 0;
}


main함수는 사용자의 명령을 받아서 long_if 함수에 전달하고 long_if 함수는 if-else를 써서 각 명령에 따라 해야할 일을 합니다. switch-case문도 결국은 if-else와 같으니까 하나로 생각해도 좋습니다.

사용자가 입력할 수 있는 명령어가 총 5가지이고, 에러처리까지 생각하면 총 6개의 경우가 생깁니다. 그래서 if-else 문도 그만큼 길어졌습니다.

언제나 그렇듯이 처음 프로그램을 기획하고 구현할때는 이정도로 충분할거라고 생각하지만, 항상 프로그램의 스펙은 변하게되어있습니다. 만약 프로그램 출시까지 스펙이 변하지않더라도, 프로그램을 버전업하면서 기능을 추가하다보면 반드시 사용자의 명령이 추가되고, if-else 문은 점점 더 길어지겠지요.

물론 if-else 문 안에서 처리해야할 일들을 따로 함수로 호출한다고 해도 다음과 같이 if-else문이 길어지는 것은 피하지 못합니다.

if case1
	call case1-handler
else if case2
	call case2-handler
else if case3
	call case3-handler
else
	call error-handler

이렇게 시간이 갈수록 길어지는 if-else 처리를 어떻게하면 간결하고, 유지보수가 편리하도록 바꿀 수 있을까요?

한가지 방법은 프로그램이 지원하는 명령과 각 명령에따라 실행되야할 코드를 따로 분리해서 관리하는 것입니다.

우선 두개의 필드를 가지는 구조체를 만듭니다.
사용자가 입력한 명령
프로그램이 실행해야할 코드

그리고 프로그램이 지원하는 명령들과 실행해야할 코드들의 짝을 미리 저장해놓습니다.
(a, handler_a)
(b, handler_b)
(c, handler_c)
(d, handler_d)
(e, handler_e)

프로그램은 사용자의 입력을 받은 후에 미리 저장해놓은 (명령,코드) 집합 중에서 어떤 명령에 해당되는지를 찾아서 코드를 실행하기만 하면 됩니다. 만약 사용자가 지원되지않는 명령을 입력했다면 에러를 반환하면 됩니다.


static int handler_a(char cmd)
{
	printf("command is a\n");
	return cmd - 'a' + 1;
};

static int handler_b(char cmd)
{
	printf("command is b\n");
	return cmd - 'a' + 1;
};

static int handler_c(char cmd)
{
	printf("command is c\n");
	return cmd - 'a' + 1;
};

static int handler_d(char cmd)
{
	printf("command is d\n");
	return cmd - 'a' + 1;
};

static int handler_e(char cmd)
{
	printf("command is e\n");
	return cmd - 'a' + 1;
};

struct cmd_handler {
	char cmd;
	int (*handler)(char);
};

int short_if(char cmd)
{
	int i;
	int ret = -1;
	struct cmd_handler chandlers[] = {
		{'a', handler_a},
		{'b', handler_b},
		{'c', handler_c},
		{'d', handler_d},
		{'e', handler_e}};
	
	for (i = 0; i < sizeof(chandlers)/sizeof(chandlers[0]); i++) {
		if (chandlers[i].cmd == cmd) {
			ret = chandlers[i].handler(chandlers[i].cmd);
			break;
		}
	}

	if (ret < 0)
		printf("Unidentified command\n");
	return ret;
}


사용자 명령을 처리하는 함수가 short_if 함수로 바뀌었습니다. short_if 함수를 보면 지역변수를 선언한 부분들이 바로 (명령,코드) 집합을 만드는 부분입니다. 

for 루프는 (명령,코드) 집합중에 사용자의 입력과 일치하는 항목이 있는지 검색합니다. 데이터와 사용자 입력을 비교해서 실행할 코드를 찾는 행위는 변하지 않습니다. 그러므로 앞으로 프로그램의 스펙이 바뀌거나 어떤 요구사항들이 들어와도 이 코드는 잘 변하지 않습니다.

사실 이러한 코드를 처음 접해보면 코드가 더 복잡해졌다고 생각할 수 있습니다. 하지만 우리가 만든 프로그램은 계속 살아서 자라난다는걸 생각해야합니다. 프로그램의 스펙은 바뀌고, 기능은 늘어납니다. 처음 기획한 명령어는 (a,b,c,d,e)일 수 있습니다. 하지만 시간이 지나면서 지원해야할 명령어가 (a,b,c,d,f)가 될 수 있습니다. long_if 함수에서 바뀐 명령어를 처리하려면 보통 if-else 코드를 찾아서 함수 중간에 있는 조건문 중 하나를 바꾸고, 코드를 바꿉니다. 그 과정에서 몇번의 빌드 에러와 런타임 에러가 발생하고 이런저런 문제들이 발생합니다. 그 문제들을 수정하다가 명령 a를 제거하고 명령g를 추가해야한다는 요구사항을 받게됩니다. 그러다보면 if-else 코드는 누더기가 되고, 어디선가 메모리가 새고 알수없는 문제들이 생기게됩니다.

short_if 함수를 보면 처리해야할 명령어가 바뀌어도 (명령어,코드) 쌍으로된 집합만 바뀌고, 사용자 명령을 받아들이고 명령어 집합을 검색해서 실행해야할 코드를 찾는 for 루프는 바뀌지 않습니다. 프로그램의 스펙이 바뀌고, 명령어가 추가되거나 삭제되어도 short_if 함수의 핵심 코드는 바뀌지 않습니다. 단지 (명령어,코드) 쌍을 추가할 뿐입니다. 명령어를 제거할 때도 (명령어,코드) 집합에 있는 항목중 하나만 지우면 됩니다. 핸들러 함수를 바로 제거하지않아도 됩니다. 왜냐면 언제나 그렇듯이 곧 지웠던 명령어를 다시 추가하라는 요구사항이 들어올거니까요. 기획자들은 늘 그랬습니다.

오랫동안 유지보수되면서 살아 숨쉬는 프로그램을 만들기 위해서는 프로그램의 디자인을 잘 해야합니다. 프로그램의 디자인을 말할때 보통 프로그램 전체의 구조를 잘 잡는 것을 의미합니다. 하지만 그것이 전부가 아닙니다. 프로그램의 코드 자체가 프로그램 전체 구조를 잘 지원할 만큼 유연해야합니다. 코드 자체의 구조 또한 잘 디자인해야 살아 숨쉬고 유연한 프로그램을 만들 수 있습니다. 잘 디자인된 프로그램이라도 막상 코드 자체에 유연성이 없으면 그런 디자인을 제대로 표현하지못하고, 유지보수가 어려울 수밖에 없습니다.

제가 생각할 때 유연한 코드가 되기 위해서는 변하는 것과 변하지않는 것을 잘 구분해야합니다. 앞으로 이 책에서 다루게될 주제들은 모두 “변하는 것과 변하지않는 것을 구분”하는 연습들입니다.

if-else 코드를 변하는 것과 변하지않는 것으로 나눠볼까요? 먼저 사용자가 입력할 명령들과 각 명령마다 실행되야할 코드들, 이런 것들은 언제든지 변하는 것입니다. 이런 것들을 일종의 데이터라고 생각할 수 있습니다. 프로그램은 코드와 데이터로 나눠지고 코드는 실행될 명령어의 집합이고, 데이터는 코드가 참고할 값들이라는 사전적인 의미에서의 코드와 데이터를 말하는 것이 아닙니다. 데이터는 변하는 것입니다. 보통 변수의 값이나 메모리 값들 등을 데이터라고 말합니다. 사진 데이터, 음악 데이터들 변경될 수 있는 값들의 집합입니다. 좀더 넓은 의미에서 생각해보면 함수 또한 데이터입니다. 언제든 변할 수 있고, 없어지거나 추가될 수 있습니다. 함수들의 집합도 데이터가 됩니다.

그럼 변하지 않는 것은 뭐가 있을까요? 사용자가 명령어를 입력한다는 것은 변하지 않습니다. 그리고 사용자의 명령에 따라 뭔가를 실행해야한다는 것도 변하지 않습니다. “사용자의 명령에 따라 움직인다”라는 사실은 우리가 만든 예제 프로그램의 존재 이유이므로 앞으로 이 프로그램에 스펙이 바뀌거나 기능이 추가되어도 변하지 않습니다.

각 명령어의 핸들러 함수들을 보면 모두 같은 타입이고 이름만 다릅니다. 공통되고 반복되는 것 또한 변하지않는 것입니다. 아래처럼 변하지않는 것을 수행해주는 매크로 함수를 만들면 앞으로 새로운 명령어와 핸들러 함수를 추가할 때 좀더 편리하겠지요.

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

struct cmd_handler {
	char cmd;
	int (*handler)(char);
};

int macro_if(char cmd)
{
	int i;
	int ret = -1;
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

	if (ret < 0)
		printf("Unidentified command\n");
	return ret;
}



연습문제
struct cmd_handler가 전역적으로 선언되었습니다. 이 구조체를 함수 안으로 집어넣을 수 있을까요? 만약 가능하다면 어떤 장점이 있을까요?
struct cmd_handler chandlers[]는 함수안에서 지역변수로 정의되어서 사용되고있습니다. 함수밖에서는 사용할 수 없겠지요. 다른 코드에서도 chandlers를 사용해야한다고 가정하고, chandlers를 함수밖에서 전역변수로 정의해보세요. 같은 파일이 아니라 다른 파일에 chandlers를 정의해보세요. 다른 파일에 chandlers를 정의하면 어떤게 편리해질까요? 사용자 명령이 5개가 아니라 10개나 50개 정도가 된다면 chandlers를 어떻게 정의하는게 좋을까요?
cmd_handler라는 구조체는 데이터의 형태를 표현한 코드입니다. 만약 데이터가 바뀌면 구조체도 바뀌어야할테고, for 루프 또한 바뀔수밖에 없습니다. 위에서 for 루프는 코드이며 바뀌지않는다고 설명했지만 for 루프 내부에 바뀔 수밖에 없는 코드가 하나 존재하는데 바로 사용자 명령과 구조체의 값을 비교하는 if (chandlers[i].cmd == cmd) 부분입니다. 이 부분을 별도의 함수로 만들어보세요. 그리고 사용자 명령의 데이터 타입을 char에서 char *로 바꿔보세요. 사용자가 ‘a’라는 한 문자를 입력하는게 아니라 “first”, “second”, “third”같은 문자열을 입력한다고 가정하고 cmd_handler 구조체를 바꾸고, if (chandlers[i].cmd == cmd) 구문을 별도의 함수로 바꿔보세요.
DEFINE_HANDLER 매크로가 최종적으로 생성하는 코드는 gcc -E 명령으로 확인할 수 있습니다. DEFINE_HANDLER(e) 코드가 어떤 코드로 변할지 종이에 한번 써보시고 gcc -E 를 실행해서 비교해보세요.
DEFINE_HANDLER 매크로는 함수의 바디까지 미리 정의합니다. 왜냐하면 모든 명령어 핸들러가 똑같은 일을 하기 때문입니다. 현실적으로 이런 경우는 드물겠지요. 하지만 핸들러 함수의 타입이 동일한 경우는 흔합니다. 사용자가 입력하는 데이터 타입이 동일하므로 모든 핸들러 함수가 동일한 타입의 데이터를 입력받고, 동일한 타입의 에러를 반환하기 때문입니다. DEFINE_HANDLER 매크로를 함수의 타입만 정의하도록 바꿔보세요. 그리고 각 핸들러마다 다른 함수 바디를 갖도록 고쳐보세요.
