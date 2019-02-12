#include <stdio.h>

struct serial_info {
	char *data;
	int digit;
	int (*put)(struct serial_info *info);
	char *(*get)(struct serial_info *info);
};

#define DEFINE_SERIALINFO(__name, __struct_info)	\
	void register_##__name(void) {					\
		register_info(#__name, &__struct_info);	\
	}

#define REGISTER_SERIALINFO(__name)    \
	void register_##__name(void);	 \
	register_##__name()

int register_info(char *name, struct serial_info *info);
int create_serial(void);
