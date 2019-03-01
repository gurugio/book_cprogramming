# Make if-else short

For example, there is a program to get a command among (a, b, c, d, e) from a user, and then print the given command.
It would be like following.

```
if cmd == ‘a’
	do_something_for_a();
else if cmd == ‘b’
	do_something_for_b();
......(skip)
else
	do_error();
```

One if-statement handles one command.
So there should be five if-statements and the last else-statement is the error case.
Let us make a real program.

```c
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
```

The long-if function checks what the user's command is and does something according to the user's command.
I used switch-case statement because many experienced programmers use switch-case to make code better.
But what is different?

Not only switch-case but also if-else handle user's command one-by-one.
One if or switch statement handles one command.
They are both long.

Yes, it is not a big deal if a program is long.
Program becomes always long.
But also program lives long.
What if specification of the program is changed?
What if program should handle more commands?
What if the name of user commands are changed, not (a,b,c,d,e), but (1,2,3,4,5)?

Whenever something is changed, we should check the entire if-else list and find out what should be changed.
Sometimes all if-else list should be changed and sometimes some.
Or sometimes it should be longer.
Usually program lives longer and program has more features.
So if-else list will be longer and longer.
And it will be more and more difficult to maintain the if-else list.

Somebody try to refine the long if-else as following.

```
if case1
	call case1-handler
else if case2
	call case2-handler
else if case3
	call case3-handler
else
	call error-handler
```

It can reduce some lines but it is still not easy to add, remove and change cases.

Now I would like to introduce one technique to separate the data and code.
The data means something given from outside and flexible. It is changed often.
The code is program code to handle the data. It is not flexible and not changed often.

We can define the code and data as following.
* data: the user commands
* code: program code to handle the user command

How can we separate the code and data?

We can define the data as following:
* (a, handler_a)
* (b, handler_b)
* (c, handler_c)
* (d, handler_d)
* (e, handler_e)

We make a list of pairs (user command, handler code for each user command) which could be called as "Data Structure".
The handler code for each user command also is called as data because it is changed when the user command is changed or program spec is changed.

Then we can make a code as following:
```
for each pair in the list
	if user's command == command in the pair
		call handler in the pair
```

This code is not changed (maybe forever).
Even if program spec is changed and many new commands are added, or many commands are removed, above code is not changed.

Let me show a program that implements the code and data seperation.

```c
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
```

I replace the long_if with short_if.
short_if defines the DATA inside of it.
The pair (command, handler) is defined as struct cmd_handler.
The the list of the pair is defined as an array of the structure.
(Of course, it can be defined as list. I just try to keep simple.)

Let us imagine if we should add some commands.
We do not need to add more if-statements.
We just add more pairs.
And if we should remove some commands, we only remove some pairs.
If we change user commands, AGAIN, we only change the pairs.
Yes, only the pairs is changed at anyhow.

Some of you might think the code becomes more complicated.
But we should remember that the program lives long and gets bigger.
The spec of the program might be (a,b,c,d,e) but it would be (a,b,c,d,f) in the future.
And it could be (a,b,c,d,f,1,2,3,4,5) in the futher future..
Whenever the spec is changed, you should look up the if-else code and which if-statement handles which command.
And someday the if-else code will be hundreds lines.

After the code and data is seperated, you will look into only the data.
The code is not changed.
If you should add some commands, you just add some pairs of (command, handler) and make some handler functions.
If the handler functions become big, you can seperate them to another file or other files.
If it is necessary to remove some commands, you just remove some pairs of (command, handlers).
It is ok if you forget to remove the handler functions.

Program design is essential to make a program that live long and evolve continuosly.
But you cannot design flexible and robust program if you do not know how to write flexible code.
At the beginning stage, the high level design could be beatiful and the program could be implemented just like the design.
But the maintaining of the program would be caos if the code is not flexible.
The design of the code is also essential to make a good program.

My idea is "It is important to separate the code (un-changable logic) and data (changable and flexible contents)".
Usuall people call the value of variable or memory contents data.
But the function could also be data often if it has dependency on data.
The set of command handlers from the above example was data.
They are functions but they can be changed: added or removed.

Then what is code?
The logic is usually code.
First it is fixed logically that the user inputs command.
Second it is also fixed that the program should do something different for each command.
They are not changed until the program is extincted.
Even-if the spec is changed, the code is not changed.

If you look at the code closely, you can see some patterns, and vice versa.
For example, the handler functions have the same argument and the name of the function also has a pattern.
We can assume that the argument and some part of the function name are not changable: they are code.
And then the argument value is data.

Following is an example to separate code and data.
The code is function namd and argument, so they are implemented by macro function.
Macro function is good to make something repeatly, so it is good tool to implement a code pattern.
And the argument for the macro function will be data.

```
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
```

## 연습문제
* struct cmd_handler가 전역적으로 선언되었습니다. 이 구조체를 함수 안으로 집어넣을 수 있을까요? 만약 가능하다면 어떤 장점이 있을까요?
* struct cmd_handler chandlers[]는 함수안에서 지역변수로 정의되어서 사용되고있습니다. 함수밖에서는 사용할 수 없겠지요. 다른 코드에서도 chandlers를 사용해야한다고 가정하고, chandlers를 함수밖에서 전역변수로 정의해보세요. 같은 파일이 아니라 다른 파일에 chandlers를 정의해보세요. 다른 파일에 chandlers를 정의하면 어떤게 편리해질까요? 사용자 명령이 5개가 아니라 10개나 50개 정도가 된다면 chandlers를 어떻게 정의하는게 좋을까요?
* cmd_handler라는 구조체는 데이터의 형태를 표현한 코드입니다. 만약 데이터가 바뀌면 구조체도 바뀌어야할테고, for 루프 또한 바뀔수밖에 없습니다. 위에서 for 루프는 코드이며 바뀌지않는다고 설명했지만 for 루프 내부에 바뀔 수밖에 없는 코드가 하나 존재하는데 바로 사용자 명령과 구조체의 값을 비교하는 if (chandlers[i].cmd == cmd) 부분입니다. 이 부분을 별도의 함수로 만들어보세요. 그리고 사용자 명령의 데이터 타입을 char에서 char *로 바꿔보세요. 사용자가 ‘a’라는 한 문자를 입력하는게 아니라 “first”, “second”, “third”같은 문자열을 입력한다고 가정하고 cmd_handler 구조체를 바꾸고, if (chandlers[i].cmd == cmd) 구문을 별도의 함수로 바꿔보세요.
* DEFINE_HANDLER 매크로가 최종적으로 생성하는 코드는 gcc -E 명령으로 확인할 수 있습니다. DEFINE_HANDLER(e) 코드가 어떤 코드로 변할지 종이에 한번 써보시고 gcc -E 를 실행해서 비교해보세요.
* DEFINE_HANDLER 매크로는 함수의 바디까지 미리 정의합니다. 왜냐하면 모든 명령어 핸들러가 똑같은 일을 하기 때문입니다. 현실적으로 이런 경우는 드물겠지요. 하지만 핸들러 함수의 타입이 동일한 경우는 흔합니다. 사용자가 입력하는 데이터 타입이 동일하므로 모든 핸들러 함수가 동일한 타입의 데이터를 입력받고, 동일한 타입의 에러를 반환하기 때문입니다. DEFINE_HANDLER 매크로를 함수의 타입만 정의하도록 바꿔보세요. 그리고 각 핸들러마다 다른 함수 바디를 갖도록 고쳐보세요.
