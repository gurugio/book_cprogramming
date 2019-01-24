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

int main(int argc, char *argv[])
{
	char cmd;

	if (argc != 2) {
		printf("usage: ./a.out command(a|b|c|d|e)\n");
		return 1;
	}

	printf("result=%d\n", long_if((char)argv[1][0]));
	printf("result=%d\n", short_if((char)argv[1][0]));
	printf("result=%d\n", macro_if((char)argv[1][0]));
	return 0;
}
