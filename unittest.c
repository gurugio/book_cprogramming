#include <stdio.h>
#include <stdlib.h>


struct unittest {
	void *private;
	int (*init)(void *);
	int (*final)(void *);
	int (*run)(void *);
	int result;
};

struct unittest_set {
	#define SET_MAX 256
	int count;
	char *names[SET_MAX];
	struct unittest *tests[SET_MAX];
};

#define REGISTER_UNITTEST(__name) \
	register_##__name();


int register_test(const char *name, struct unittest)
{

}

int run_test(void)
{

}
