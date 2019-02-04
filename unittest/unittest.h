#include <stdio.h>

struct unittest {
	int (*init)(void *);
	int (*final)(void *);
	int (*run)(void *);
	void *priv;
};

struct unittest_set {
	#define SET_MAX 256
	int count;
	char *names[SET_MAX];
	struct unittest *tests[SET_MAX];
};

#define DEFINE_UNITTEST(__name) \
	void register_##__name(void)

#define REGISTER_UNITTEST(__name)    \
	void register_##__name(void); \
	register_##__name()

int register_test(char *name, struct unittest *test);
int run_test(void);
