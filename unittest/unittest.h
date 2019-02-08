#include <stdio.h>

struct unittest {
	int (*init)(void *);
	int (*final)(void *);
	int (*run)(void *);
	void *priv;
};

#define DEFINE_UNITTEST(__name, __struct_unittest)	\
	void register_##__name(void) {					\
		register_test(#__name, &__struct_unittest);	\
	}

#define REGISTER_UNITTEST(__name)    \
	void register_##__name(void);	 \
	register_##__name()

int register_test(char *name, struct unittest *test);
int run_test(void);
